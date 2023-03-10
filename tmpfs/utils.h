#include "structs.h"

#include <fnmatch.h>
#include <string.h>

#define BLOCK_INDEX_DEFAULT -1

void nodes_init(storage_t storage) {
	for (int i = 0; i < storage.nodex_max; i++) {
		storage.nodes[i].size = 0;
		storage.nodes[i].start_block = BLOCK_INDEX_DEFAULT;
		storage.nodes[i].is_dir = 0;
	}
}

void blocks_init(storage_t storage) {
	for (int i = 0; i < storage.blocks_max; i++) {
		storage.blocks[i].next_block = BLOCK_INDEX_DEFAULT;
	}
}


tmpfs_inode* get_free_node(storage_t storage) {
	for (int i = 0; i < storage.nodex_max; i++) {
		if ( storage.nodes[i].used == 0 ){
			storage.nodes[i].used = 1;
			return &storage.nodes[i];
		}
	}

	return NULL;
}

int get_free_block_index(storage_t storage) {
	for (int i = 0; i < storage.blocks_max; i++) {
		if ( storage.blocks[i].used == 0 ){
			storage.blocks[i].used = 1;
			return i;
		}
	}

	return -EXIT_FAILURE;
}

int subdir(const char *dir, const char *path) {
    char a_path[PATH_MAX];
	strcpy(a_path, dir);
	strcat(a_path, "/*");

	return fnmatch(a_path, path, FNM_PATHNAME);
}

int find_node_index(storage_t storage, const char *path) {
	for (int i = 0; i < storage.nodex_max; i++) {
		if (storage.nodes[i].used == 1 && strcmp(path, storage.nodes[i].path) == 0) {
			return i;
		}
	}

	return -ENOENT;
}

int free_block(storage_t storage, int index) {
	int next = storage.blocks[index].next_block;
	storage.blocks[index].next_block = BLOCK_INDEX_DEFAULT;

	return next;
}