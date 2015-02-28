/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015 Narfinger
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

#ifndef GITCOMMIT_H
#define GITCOMMIT_H

#include "gitrepository.h"

class GitCommit {
public:
  GitCommit(git_commit* c, GitRepositoryPtr repo);
  ~GitCommit();
  const QDateTime time() const;
  const QString file(const QString& filename) const;
  
private:
  GitCommit() {};
  git_commit* commit_;
  GitRepositoryPtr repo_;
  const QString readBuffer(const void*, const int size) const; //rawcontent and size
};

#endif // GITCOMMIT_H
