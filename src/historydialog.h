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

#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>

#include "gitrepository.h"

#include "ui_history.h"

class HistoryDialog : public QDialog
{
    Q_OBJECT
public:
  HistoryDialog(GitRepositoryPtr repo, QWidget* parent = nullptr);

private:
    Ui::HistoryDialog ui_;

    GitRepositoryPtr repo_;
    QList<GitCommitPtr> history_;
    //QSharedPointer<SpikesTreeModel> stm_;

    void changeDate(const int row);
};

#endif // HISTORY_H
