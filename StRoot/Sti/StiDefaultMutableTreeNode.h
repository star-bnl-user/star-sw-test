#ifndef StiDefaultMutableTreeNode_H
#define StiDefaultMutableTreeNode_H 1

#include <iostream.h>
#include <stdlib.h>
#include "TObject.h"
#include "TObjArray.h"
#include "StiTreeNode.h"

//-----------------------------------------------------------------------------
// A <code>DefaultMutableTreeNode</code> is a general-purpose node in a tree data
// structure. A tree node may have at most one parent and 0 or more children.
// <code>DefaultMutableTreeNode</code> provides operations for examining and modifying a
// node's parent and children and also operations for examining the tree that
// the node is a part of.  A node's tree is the set of all nodes that can be
// reached by starting at the node and following all the possible links to
// parents and children.  A node with no parent is the root of its tree; a
// node with no children is a leaf.  A tree may consist of many subtrees,
// each node acting as the root for its own subtree.
// <p>
// While DefaultMutableTreeNode implements the MutableTreeNode interface and
// will allow you to add in any implementation of MutableTreeNode not all
// of the methods in DefaultMutableTreeNode will be applicable to all
// StiTreeNodes implementations. Especially with some of the enumerations
// that are provided, using some of these methods assumes the
// DefaultMutableTreeNode contains only DefaultMutableNode instances. All
// of the TreeNode/MutableTreeNode methods will behave as defined no
// matter what implementations are added.
// <p>
//
// @see StiTreeNode
//
//-----------------------------------------------------------------------------

class StiDefaultMutableTreeNode : public StiTreeNode
{
 public:

  StiDefaultMutableTreeNode();
  StiDefaultMutableTreeNode(TObject *  userObject) ;
  StiDefaultMutableTreeNode(TObject *  userObject, bool allowsChildren);
  StiDefaultMutableTreeNode(TObject *  userObject, int childrenArraySize);
  StiDefaultMutableTreeNode(TObject *  userObject, bool allowsChildren, int childrenArraySize);
  void initialize(TObject *  userObject, bool allowsChildren, int childrenArraySize);
  void insert(StiTreeNode * newChild, int childIndex);
  void remove(int childIndex) ;
  void setParent(StiTreeNode *  newParent) ;
  StiTreeNode *  getParent() ;
  StiTreeNode *  getChildAt(int index) ;
  int getChildCount() ;
  int getIndex(StiTreeNode *  aChild) ;
  void setAllowsChildren(bool allows);
  bool getAllowsChildren() ;
  void setUserObject(TObject *  userObject);
  TObject *  getUserObject() ;
  void   removeFromParent() ;
  void remove(StiTreeNode *  aChild) ;
  void removeAllChildren();
  void removeAllChildrenBut(StiTreeNode *  aChild);
  void add(StiTreeNode *  newChild);
  bool isNodeAncestor(StiTreeNode *  anotherNode) ;
  bool isNodeDescendant(StiDefaultMutableTreeNode *  anotherNode) ;
  StiTreeNode *  getSharedAncestor(StiDefaultMutableTreeNode *  aNode);
  bool isNodeRelated(StiDefaultMutableTreeNode *  aNode) ;
  int getDepth() ;
  int getLevel() ;
  StiTreeNode *  getRoot() ;
  bool isRoot();
  StiDefaultMutableTreeNode *  getNextNode() ;
  StiDefaultMutableTreeNode *  getPreviousNode(); 
  bool isNodeChild(StiTreeNode *  aNode) ;
  StiTreeNode *  getFirstChild() ;
  StiTreeNode *  getLastChild() ;
  StiTreeNode *  getChildAfter(StiTreeNode *  aChild) ;
  StiTreeNode *  getChildBefore(StiTreeNode *  aChild) ;
  bool isNodeSibling(StiTreeNode *  anotherNode) ;
  int getSiblingCount() ;
  StiDefaultMutableTreeNode *  getNextSibling() ;
  StiDefaultMutableTreeNode *  getPreviousSibling() ;
  bool isLeaf();
  StiDefaultMutableTreeNode *  getFirstLeaf();
  StiDefaultMutableTreeNode *  getLastLeaf(); 
  StiDefaultMutableTreeNode *  getNextLeaf();
  StiDefaultMutableTreeNode *  getPreviousLeaf() ;

 protected:

  bool        allowsChildren;
  TObject   * userObject;
  TObjArray * children;
  //StiDefaultMutableTreeNode * parent;
  StiTreeNode * parent;

  ClassDef(StiDefaultMutableTreeNode,1)

};

#endif


