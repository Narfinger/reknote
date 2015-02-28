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
#include <QStandardItem>

#include "historydialog.h"
#include "spikestreemodel.h"

HistoryDialog::HistoryDialog(GitRepositoryPtr repo, QWidget* parent): repo_(repo), QDialog(parent) {
  ui_.setupUi(this);

  history_ = repo_->walkHistory(repo_);
  for(GitCommitPtr p: history_) {
    ui_.dateList->addItem(p->time().toString(Qt::ISODate));
  }
  stm_ = QSharedPointer<SpikesTreeModel>(new SpikesTreeModel());
  ui_.spikestreeview->setModel(stm_.data());

  connect(ui_.dateList, &QListWidget::currentRowChanged, this, &HistoryDialog::changeDate);
}

void HistoryDialog::changeDate(const int row) {
  stm_->clear();
  const QString file = history_.at(row)->file("CURRENTLY IGNORED");
  qDebug() << file;
  stm_->loadGitCommit(history_.at(row));
}

