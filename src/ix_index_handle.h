//
// ix_file_handle.h
//
//   Index Manager Component Interface
//

#ifndef IX_FILE_HANDLE_H
#define IX_FILE_HANDLE_H

#include "redbase.h"  // Please don't change these lines
#include "rm_rid.h"  // Please don't change these lines
#include "pf.h"
#include "ix_error.h"
#include "btree_node.h"
//
// IX_FileHdr: Header structure for files
//
struct IX_FileHdr {
  int numPages;      // # of pages in the file
  int pairSize;      // size of each (key, RID) pair in index
  int order;         // order of btree
  int height;        // height of btree
  AttrType attrType;
  int attrLength;
};

const int IX_PAGE_LIST_END = -1;

//
// IX_IndexHandle: IX Index File interface
//
class IX_IndexHandle {
  friend class IX_Manager;
  friend class IX_IndexHandleTest;
  friend class BtreeNodeTest;

 public:
  IX_IndexHandle();
  ~IX_IndexHandle();
  
  // Insert a new index entry
  RC InsertEntry(void *pData, const RID &rid);
  
  // Delete a new index entry
  RC DeleteEntry(void *pData, const RID &rid);
  
  // Force index files to disk
  RC ForcePages();

 private:
  RC Open(PF_FileHandle * pfh, int pairSize);
  RC GetFileHeader(PF_PageHandle ph);
  // persist header into the first page of a file for later
  RC SetFileHeader(PF_PageHandle ph) const;

  bool HdrChanged() const { return bHdrChanged; }
  int GetNumPages() const { return hdr.numPages; }
  RC GetNewPage(PageNum& pageNum);

 private:
  IX_FileHdr hdr;
  bool bFileOpen;
  PF_FileHandle * pfHandle;
  bool bHdrChanged;
  BtreeNode * root; // root in turn points to the other nodes
  BtreeNode ** path; // list of nodes that is the path to leaf as a
                     // result of a search.
};

#endif // #IX_FILE_HANDLE_H
