/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  Narfinger.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
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

#ifndef NOTEITEMDELEGATE_H
#define NOTEITEMDELEGATE_H

#include <memory>
#include <QStyledItemDelegate>
class QTextDocument;
class QWidget;

class NoteItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
  NoteItemDelegate(QObject * parent = 0);
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
  std::unique_ptr<QTextDocument> html(const QStyleOptionViewItem& option, const QModelIndex &index) const;
};

#endif // NOTEITEMDELEGATE_H
