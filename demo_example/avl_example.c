
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include "avl_tree.h"

typedef struct {
    char line[128];
    ZDFSTrunkPathInfo path;  //trunk file path
    int id;                  //trunk file id
} ZDFSTrunkFileIdInfo;

/* 比较函数,先比较ZDFSTrunkPathInfo里的内容,如果相同再比较ZDFSTrunkFileIdInfo.id
 *
 * */
static int storage_compare_trunk_id_info(void *p1, void *p2)
{
    int result;
    result = memcmp(&(((ZDFSTrunkFileIdInfo *)p1)->path), \
			&(((ZDFSTrunkFileIdInfo *)p2)->path), \
			sizeof(ZDFSTrunkPathInfo));
    if (result != 0)
    {
        return result;
    }

    return ((ZDFSTrunkFileIdInfo *)p1)->id - ((ZDFSTrunkFileIdInfo *)p2)->id;
}

static int tree_write_file_walk_callback(void *data, void *args)
{
    int result;

    if (fprintf((FILE *)args, "%s\n", \
		((ZDFSTrunkFileIdInfo *)data)->line) > 0)
    {
        return 0;
    }
    else
    {
        result = errno != 0 ? errno : EIO;
        logError("file: "__FILE__", line: %d, " \
			"write to binlog file fail, " \
			"errno: %d, error info: %s.", \
			__LINE__, result, STRERROR(result));
        return EIO;
    }
}

/*例子*/
int main()
{
    AVLTreeInfo tree_unique_trunks;
    //进行avl的初始化
    avl_tree_init(&tree_unique_trunks, free, storage_compare_trunk_id_info);

    //avl 数据查找
    ZDFSTrunkFileIdInfo trunkFileId;
    trunkFileId.path = "/opt/";
    trunkFileId.id = 1;
    ZDFSTrunkFileIdInfo *pFound = (ZDFSTrunkFileIdInfo *)avl_tree_find(&tree_unique_trunks, &trunkFileId);

    //avl 插入,插入成功返回1
    pFound = (ZDFSTrunkFileIdInfo *)malloc(sizeof(ZDFSTrunkFileIdInfo));
    if(avl_tree_insert(&tree_unique_trunks, pFound) != 1)
    {
        result = errno != 0 ? errno : ENOMEM;
        logError("file: "__FILE__", line: %d, " \
					"avl_tree_insert fail, " \
					"errno: %d, error info: %s", \
					__LINE__, result, STRERROR(result));
        break;
    }

    //记录avl节点的个数
    int tree_node_count;
    tree_node_count = avl_tree_count(&tree_unique_trunks);

    //对每个节点进行相关的操作
    int result = avl_tree_walk(&tree_unique_trunks, tree_write_file_walk_callback, fp);

    //释放avl树
    avl_tree_destroy(&tree_unique_trunks);
}
