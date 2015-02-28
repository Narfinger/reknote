/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Narfinger
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "gitcommit.h"

GitCommit::GitCommit(git_commit* c, GitRepositoryPtr repo) : commit_(c), repo_(repo) {
}

GitCommit::~GitCommit() {
  git_commit_free(commit_);
}

const QDateTime GitCommit::time() const {
  QDateTime t;
  t.setTime_t(git_commit_time(commit_));
  return t;
}

const QString GitCommit::file(const QString& filename) const {
  git_tree_entry* entry = nullptr;
  git_tree* tree = nullptr;
  int error = git_commit_tree(&tree, commit_);
  gitErrorCheck(error);
  
  error = git_tree_entry_bypath(&entry, tree, "spikestree.xml");
  gitErrorCheck(error);
  if (error < 0) return QString();
  const git_oid* oid = git_tree_entry_id(entry);
  git_blob* blob = nullptr;
  error = git_blob_lookup(&blob, repo_->repo_, oid);
  gitErrorCheck(error);
  const void* raw = git_blob_rawcontent(blob);
  const int size = git_blob_rawsize(blob);
  
  //read buffer
  const QString result = readBuffer(raw, size);
  

  git_tree_free(tree);
  git_blob_free(blob);
  
  return result;
}

const QString GitCommit::readBuffer(const void* buf, const int size) const {
  const QByteArray ba((char*)buf, size);
  return QString(ba);
}
