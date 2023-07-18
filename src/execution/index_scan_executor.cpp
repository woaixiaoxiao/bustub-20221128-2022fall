//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// index_scan_executor.cpp
//
// Identification: src/execution/index_scan_executor.cpp
//
// Copyright (c) 2015-19, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#include "execution/executors/index_scan_executor.h"

namespace bustub {
IndexScanExecutor::IndexScanExecutor(ExecutorContext *exec_ctx, const IndexScanPlanNode *plan)
    : AbstractExecutor(exec_ctx), plan_(plan) {}

void IndexScanExecutor::Init() {
  // 初始化B+树及B+树的两个指针
  index_oid_t index_id = plan_->GetIndexOid();
  IndexInfo *index_info = exec_ctx_->GetCatalog()->GetIndex(index_id);
  tree_ = dynamic_cast<BPlusTreeIndexForOneIntegerColumn *>(index_info->index_.get());
  cursor_ = tree_->GetBeginIterator();
  end_ = tree_->GetEndIterator();
  // 获得表的指针
  table_heap_ = exec_ctx_->GetCatalog()->GetTable(index_info->table_name_)->table_.get();
}

auto IndexScanExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (cursor_ == end_) {
    return false;
  }
  *rid = (*cursor_).second;
  table_heap_->GetTuple(*rid, tuple, exec_ctx_->GetTransaction());
  ++cursor_;
  return true;
}

}  // namespace bustub
