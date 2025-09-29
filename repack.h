#ifndef REPACK_H
#define REPACK_H

#include "list-objects-filter-options.h"
#include "string-list.h"

struct pack_objects_args {
	char *window;
	char *window_memory;
	char *depth;
	char *threads;
	unsigned long max_pack_size;
	int no_reuse_delta;
	int no_reuse_object;
	int quiet;
	int local;
	int name_hash_version;
	int path_walk;
	int delta_base_offset;
	struct list_objects_filter_options filter_options;
};

#define PACK_OBJECTS_ARGS_INIT { .delta_base_offset = 1 }

struct child_process;

void prepare_pack_objects(struct child_process *cmd,
			  const struct pack_objects_args *args,
			  const char *out);
void pack_objects_args_release(struct pack_objects_args *args);

void repack_remove_redundant_pack(struct repository *repo, const char *dir_name,
				  const char *base_name);

struct repository;
struct packed_git;

struct existing_packs {
	struct repository *repo;
	struct string_list kept_packs;
	struct string_list non_kept_packs;
	struct string_list cruft_packs;
};

#define EXISTING_PACKS_INIT { \
	.kept_packs = STRING_LIST_INIT_DUP, \
	.non_kept_packs = STRING_LIST_INIT_DUP, \
	.cruft_packs = STRING_LIST_INIT_DUP, \
}

/*
 * Adds all packs hex strings (pack-$HASH) to either packs->non_kept
 * or packs->kept based on whether each pack has a corresponding
 * .keep file or not.  Packs without a .keep file are not to be kept
 * if we are going to pack everything into one file.
 */
void existing_packs_collect(struct existing_packs *existing,
			    const struct string_list *extra_keep);
int existing_packs_has_non_kept(const struct existing_packs *existing);
int existing_pack_is_marked_for_deletion(struct string_list_item *item);
void existing_packs_retain_cruft(struct existing_packs *existing,
				 struct packed_git *cruft);
void existing_packs_mark_for_deletion(struct existing_packs *existing,
				      struct string_list *names);
void existing_packs_remove_redundant(struct existing_packs *existing,
				     const char *packdir);
void existing_packs_release(struct existing_packs *existing);

struct generated_pack;

struct generated_pack *generated_pack_populate(const char *name,
					       const char *packtmp);
int generated_pack_has_ext(const struct generated_pack *pack, const char *ext);
void generated_pack_install(struct generated_pack *pack, const char *name,
			    const char *packdir, const char *packtmp);

#endif /* REPACK_H */
