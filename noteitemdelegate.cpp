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

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QTextDocument>

#include "noteitemdelegate.h"

NoteItemDelegate::NoteItemDelegate(QObject * parent) : 
  QStyledItemDelegate(parent) {
  }

void NoteItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index) const {
  QStyleOptionViewItem o = option;
  initStyleOption(&o, index);
  QStyle *style = o.widget->style(); //do we need this?
  QTextDocument d;
  d.setDefaultFont(o.font);
  QString text = o.text;
  if (o.checkState == Qt::Checked) {
    text = text.prepend("<s>").append("</s>");
  }
  d.setHtml(text);
  o.text = QString();
  style->drawControl(QStyle::CE_ItemViewItem, &o, painter);
  QAbstractTextDocumentLayout::PaintContext ctx;
  
  QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &o);
  painter->save();
  painter->translate(textRect.topLeft());
  d.documentLayout()->draw(painter, ctx);
  painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QStyleOptionViewItem o = option;
  initStyleOption(&o, index);
  QTextDocument d;
  d.setDefaultFont(o.font);
  d.setHtml(o.text);
  return QSize(o.widget->size().width(), d.size().height());
}
