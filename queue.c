/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "callbacks.h"
#include "err.h"
#include "fs.h"
#include "netdata.h"
#include "queue.h"

#define QMAIL_QUEUE_PATH "/var/qmail/queue/"

struct queue_statistics {
	int mess;
	int todo;
};

static
void *
queue_data_init() {
	struct queue_statistics * ret;

	if (is_directory(QMAIL_QUEUE_PATH "mess") != 1) {
		return NULL;
	}

	if (is_directory(QMAIL_QUEUE_PATH "todo") != 1) {
		return NULL;
	}

	ret = calloc(1, sizeof * ret);
	return ret;
}

static
int
print_queue_hdr(const char * name) {
	nd_chart("qmail", "queue", NULL, NULL, NULL, NULL, NULL, "qmail.queue", ND_CHART_TYPE_AREA);
	nd_dimension("mess", NULL, ND_ALG_ABSOLUTE, 1, 1, ND_VISIBLE);
	nd_dimension("todo", NULL, ND_ALG_ABSOLUTE, 1, 1, ND_VISIBLE);

	return fflush(stdout);
}

static
int
print_queue_data(const char * name, const struct queue_statistics * data, const unsigned long time) {
	nd_begin_time("qmail", "queue", NULL, time);
	nd_set("mess", data->mess);
	nd_set("todo", data->todo);
	nd_end();

	return fflush(stdout);
}

static
int
measure_dir(const char * name) {
	char path[PATH_MAX];
	struct dirent * de;
	int res = 0;
	DIR * dir;

	dir = opendir(name);
	if (dir == NULL) {
		fprintf(stderr, "Cannot open dir: %s\n", name);
		return 0;
	}

	while ((de = readdir(dir))) {
		if (de->d_name[0] == '.')
			continue;

		if (de->d_type == DT_DIR) {
			sprintf(path, "%s/%s", name, de->d_name);
			res += measure_dir(path);
		} else if (de->d_type == DT_REG) {
			res += 1;
		} else if (de->d_type == DT_UNKNOWN) {
			sprintf(path, "%s/%s", name, de->d_name);
			if (is_directory(path)) {
				res += measure_dir(path);
			} else {
				res += 1;
			}
		}
	}

	closedir(dir);

	return res;
}

static
void
measure_queue(const char * unused, struct queue_statistics * data) {
	data->mess = measure_dir(QMAIL_QUEUE_PATH "mess");
	data->todo = measure_dir(QMAIL_QUEUE_PATH "todo");
}

static
void
clear_data(struct queue_statistics * data) {
	memset(data, 0, sizeof * data);
}

static
struct stat_func queue = {
	.init = &queue_data_init,
	.fini = &free,

	.print_hdr   = &print_queue_hdr,
	.print       = (int (*)(const char *, const void *, unsigned long))&print_queue_data,
	.process     = (void (*)(const char *, void *))&measure_queue,
	.postprocess = NULL,
	.clear       = (void (*)(void *))&clear_data,
};

struct stat_func * queue_func = &queue;
