/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Narfinger <narfinger@users.github.com>
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

#ifndef SPIKENOTEWIDGET_H
#define SPIKENOTEWIDGET_H

#include "ui_spikenotewidget.h"

class SpikesTreeModel;
class GitRepository;

class SpikeNoteWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool readonly READ isReadOnly WRITE setReadOnly DESIGNABLE true)

public:
  SpikeNoteWidget(QWidget* parent = nullptr);
  ~SpikeNoteWidget();
  void addSpike();
  void deleteSelectedSpike(); 
  void deleteNote();
  void cleanDone();
  void undo() { ui_.noteView->undo(); }
  void setSpikeActions(const QList<QAction*>& list) 	{ al_ = list; }
  void spikestreeContextMenu(const QPoint& point) const;
  const SpikesTreeModel* spikestreemodel()  const { return sm_; }
  bool isReadOnly() const { return readonly_; }
  void setReadOnly(const bool r);

private:
  Ui::SpikeNoteWidget ui_;

  SpikesTreeModel* sm_;
  QList<QAction*> al_;
  bool readonly_ = false;

  void selectIndex(const QModelIndex& index);
  void activated(QModelIndex i);
};

#endif // SPIKENOTEWIDGET_H
