#include "main.hpp"
#include "config_parser.hpp"
#include "git_info.hpp"

#include <gtkmm/application.h>
#include <filesystem>
#include <dlfcn.h>

void handle_signal(int signum) {
	sysboard_handle_signal_ptr(window, signum);
}

void load_libsysboard() {
	void* handle = dlopen("libsysboard.so", RTLD_LAZY);
	if (!handle) {
		std::fprintf(stderr, "Cannot open library: %s\n", dlerror());
		exit(1);
	}

	sysboard_create_ptr = (sysboard_create_func)dlsym(handle, "sysboard_create");
	sysboard_handle_signal_ptr = (sysboard_handle_signal_func)dlsym(handle, "sysboard_signal");

	if (!sysboard_create_ptr || !sysboard_handle_signal_ptr) {
		std::fprintf(stderr, "Cannot load symbols: %s\n", dlerror());
		dlclose(handle);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	// Load the config
	std::string config_path;
	std::map<std::string, std::map<std::string, std::string>> config;
	std::map<std::string, std::map<std::string, std::string>> config_usr;

	bool cfg_sys = std::filesystem::exists("/usr/share/sys64/board/config.conf");
	bool cfg_sys_local = std::filesystem::exists("/usr/local/share/sys64/board/config.conf");
	bool cfg_usr = std::filesystem::exists(std::string(getenv("HOME")) + "/.config/sys64/board/config.conf");

	// Load default config
	if (cfg_sys)
		config_path = "/usr/share/sys64/board/config.conf";
	else if (cfg_sys_local)
		config_path = "/usr/local/share/sys64/board/config.conf";
	else
		std::fprintf(stderr, "No default config found, Things will get funky!\n");

	config = config_parser(config_path).data;

	// Load user config
	if (cfg_usr)
		config_path = std::string(getenv("HOME")) + "/.config/sys64/board/config.conf";
	else
		std::fprintf(stderr, "No user config found\n");

	config_usr = config_parser(config_path).data;

	// Merge configs
	for (const auto& [key, nested_map] : config_usr)
		for (const auto& [inner_key, inner_value] : nested_map)
			config[key][inner_key] = inner_value;

	// Sanity check
	if (!(cfg_sys || cfg_sys_local || cfg_usr)) {
		std::fprintf(stderr, "No config available, Something ain't right here.");
		return 1;
	}

	while (true) {
		switch(getopt(argc, argv, "m:H:l:vh")) {
			case 'm':
				config["main"]["margin"] = optarg;
				continue;

			case 'H':
				config["main"]["height"] = optarg;
				continue;

			case 'l':
				config["main"]["layout"] = optarg;
				continue;

			case 'v':
				std::printf("Commit: %s", GIT_COMMIT_MESSAGE);
				std::printf("Date: %s", GIT_COMMIT_DATE);
				return 0;

			case 'h':
			default :
				std::printf("usage:\n");;
				std::printf("  sysboard [argument...]:\n\n");;
				std::printf("arguments:\n");;
				std::printf("  -m	Set margin\n");;
				std::printf("  -H	Set height\n");;
				std::printf("  -l	Set layout\n");;
				std::printf("  -v	Prints version info\n");;
				std::printf("  -h	Show this help message\n");;
				return 0;

			case -1:
				break;
			}

			break;
	}

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("funky.sys64.sysboard");
	app->hold();

	load_libsysboard();
	window = sysboard_create_ptr(config);
	(void)window; // This is to avoid the unused variable warning

	// Catch signals
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	signal(SIGRTMIN, handle_signal);

	return app->run();
}
