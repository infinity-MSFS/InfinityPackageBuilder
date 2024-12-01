#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsdiff.h"


static void log_error(void *opaque, const char *errmsg) {
    (void) opaque;
    fprintf(stderr, "%s", errmsg);
}

inline int generate_patch(const char *oldname, const char *newname, const char *patchname) {
    int ret = 1;
    struct bsdiff_stream oldfile = {0}, newfile = {0}, patchfile = {0};
    struct bsdiff_ctx ctx = {0};
    struct bsdiff_patch_packer packer = {0};

    ret = bsdiff_open_file_stream(BSDIFF_MODE_READ, oldname, &oldfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open oldfile: %s\n", oldname);
        goto cleanup;
    }
    ret = bsdiff_open_file_stream(BSDIFF_MODE_READ, newname, &newfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open newfile: %s\n", newname);
        goto cleanup;
    }
    ret = bsdiff_open_file_stream(BSDIFF_MODE_WRITE, patchname, &patchfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open patchfile: %s\n", patchname);
        goto cleanup;
    }
    ret = bsdiff_open_bz2_patch_packer(BSDIFF_MODE_WRITE, &patchfile, &packer);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't create BZ2 patch packer\n");
        goto cleanup;
    }

    ctx.log_error = log_error;

    ret = bsdiff(&ctx, &oldfile, &newfile, &packer);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "bsdiff failed: %d\n", ret);
        goto cleanup;
    }

cleanup:
    bsdiff_close_patch_packer(&packer);
    bsdiff_close_stream(&patchfile);
    bsdiff_close_stream(&newfile);
    bsdiff_close_stream(&oldfile);

    return ret;
}

inline int apply_patch(const char *oldname, const char *newname, const char *patchname) {
    int ret = 1;
    struct bsdiff_stream oldfile = {0}, newfile = {0}, patchfile = {0};
    struct bsdiff_ctx ctx = {0};
    struct bsdiff_patch_packer packer = {0};

    ret = bsdiff_open_file_stream(BSDIFF_MODE_READ, oldname, &oldfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open oldfile: %s\n", oldname);
        goto cleanup;
    }
    ret = bsdiff_open_file_stream(BSDIFF_MODE_WRITE, newname, &newfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open newfile: %s\n", newname);
        goto cleanup;
    }
    ret = bsdiff_open_file_stream(BSDIFF_MODE_READ, patchname, &patchfile);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't open patchfile: %s\n", patchname);
        goto cleanup;
    }
    ret = bsdiff_open_bz2_patch_packer(BSDIFF_MODE_READ, &patchfile, &packer);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "can't create BZ2 patch packer\n");
        goto cleanup;
    }

    ctx.log_error = log_error;

    ret = bspatch(&ctx, &oldfile, &newfile, &packer);
    if (ret != BSDIFF_SUCCESS) {
        fprintf(stderr, "bspatch failed: %d\n", ret);
        goto cleanup;
    }

cleanup:
    bsdiff_close_patch_packer(&packer);
    bsdiff_close_stream(&patchfile);
    bsdiff_close_stream(&newfile);
    bsdiff_close_stream(&oldfile);

    return ret;
}

inline void create_test_file(const char *filename, const char *data, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to create file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fwrite(data, 1, size, file);
    fclose(file);
}

inline int files_are_equal(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    if (!f1 || !f2) {
        fprintf(stderr, "Failed to open files for comparison\n");
        return 0;
    }

    int equal = 1;
    int c1, c2;
    while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF) {
        if (c1 != c2) {
            equal = 0;
            break;
        }
    }
    if (fgetc(f1) != EOF || fgetc(f2) != EOF) equal = 0;

    fclose(f1);
    fclose(f2);
    return equal;
}

void TestBSDiff() {
    const char *oldfile = "oldfile.bin";
    const char *newfile = "newfile.bin";
    const char *patchfile = "patchfile.patch";
    const char *patchedfile = "patchedfile.bin";

    const char olddata[] = {1, 2, 3, 4, 5};
    const char newdata[] = {1, 2, 3, 6, 7, 8};

    create_test_file(oldfile, olddata, sizeof(olddata));
    create_test_file(newfile, newdata, sizeof(newdata));

    if (generate_patch(oldfile, newfile, patchfile) != BSDIFF_SUCCESS) {
        fprintf(stderr, "Patch generation failed\n");
    }

    if (apply_patch(oldfile, patchedfile, patchfile) != BSDIFF_SUCCESS) {
        fprintf(stderr, "Patch application failed\n");
    }

    if (files_are_equal(newfile, patchedfile)) {
        printf("Test passed: Patched file matches the new file\n");
    } else {
        printf("Test failed: Patched file does not match the new file\n");
    }

    remove(oldfile);
    remove(newfile);
    remove(patchfile);
    remove(patchedfile);

}