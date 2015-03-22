/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Christian Engels <engels@cs.uni-saarland.de>
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
public:
  SpikeNoteWidget(QWidget* parent = nullptr);
  ~SpikeNoteWidget();
  void addSpike();
  void deleteSelectedSpike(); 
  void deleteNote();
  void cleanDone();
  
  const SpikesTreeModel* spikestreemodel()  const { return sm_; }
    
private:
  Ui::SpikeNoteWidget ui_;
    
  SpikesTreeModel* sm_;
    
  void selectIndex(const QModelIndex& index);
  void activated(QModelIndex i);
};

#endif // SPIKENOTEWIDGET_H
