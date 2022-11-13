#ifndef SHARED_H
#define SHARED_H

#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>


typedef struct {
	const char *name;
	void *handle;
	unsigned long int mtime;
} SharedLib;


unsigned long int shared__get_time(const char *name) {
	struct stat s;
	stat(name, &s);
	return s.st_mtime;
}

void shared_lib_open(SharedLib *lib, const char *name) {
	lib->name = name;
	lib->handle = dlopen(lib->name, RTLD_LAZY);
	if (!lib->handle) {
		const char *error = dlerror();
		perror(error);
	}
	lib->mtime = shared__get_time(lib->name);
}

void* shared_lib_get_function(SharedLib *lib, const char *function_name) {
	void *res = dlsym(lib->handle, function_name);
	if (res == NULL) {
		const char *error = dlerror();
		perror(error);
	}
	return res;
}

void shared_lib_close(SharedLib *lib) {
	dlclose(lib->handle);
	lib->name = NULL;
	lib->mtime = 0;
}

bool shared_lib_check_and_reload(SharedLib *lib) {
	bool res = false;
	unsigned long int mtime_new = shared__get_time(lib->name);
	if (mtime_new != lib->mtime && access(".shared.lock", F_OK) != 0) {
		const char *name = lib->name;
		shared_lib_close(lib);
		shared_lib_open(lib, name);
		res = true;
	}
	return res;
}

#endif
