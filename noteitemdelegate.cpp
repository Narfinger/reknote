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
#include <QPlainTextEdit>
#include <QTextOption>

#include "noteitemdelegate.h"
#include "spike.h"

const int NoteItemDelegate::PADDING_WIDTH = 20;
const int NoteItemDelegate::EDIT_DEFAULT_SCALE = 3;
const QString NoteItemDelegate::EDIT_TEXT = "enter text";

NoteItemDelegate::NoteItemDelegate(QObject * parent) : 
  QStyledItemDelegate(parent) {
}
  
QWidget* NoteItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
  Q_UNUSED(option); Q_UNUSED(index);
  QPlainTextEdit* e = new QPlainTextEdit(parent);
  return e;
}

void NoteItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) {
  QPlainTextEdit* e = dynamic_cast<QPlainTextEdit*>(editor);
  e->setPlainText(index.data().toString());
}

void NoteItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex &index) const { 
  QStyleOptionViewItem o = option;
  initStyleOption(&o, index);

  if (o.checkState == Qt::Checked) {
    o.text = o.text.prepend("<s>").append("</s>");
  }
  const QVariant col = index.data(Spike::colorrole);
  if (col.isValid()) {
    const QString style = QString("<span style=\"background-color: %1\">").arg(col.toString());
    o.text.prepend(style).append("</span>");
  }

  std::unique_ptr<QTextDocument> d = html(o, index);
  
  //render
  o.text = QString();
  QStyle *style = o.widget->style(); //do we need this?
  style->drawControl(QStyle::CE_ItemViewItem, &o, painter);
  QAbstractTextDocumentLayout::PaintContext ctx;
  QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &o);
  painter->save();
  painter->translate(textRect.topLeft());
  d->documentLayout()->draw(painter, ctx);
  painter->restore();
}

QSize NoteItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
  QStyleOptionViewItem o = option;
  initStyleOption(&o, index);
  std::unique_ptr<QTextDocument> d = html(o, index);
  int scale = 1;
  if(o.text == EDIT_TEXT) {	//when new text we want a bit more space
    scale = EDIT_DEFAULT_SCALE;
  }
  return QSize(o.widget->size().width()-PADDING_WIDTH, d->size().height()*scale);
}

std::unique_ptr<QTextDocument> NoteItemDelegate::html(const QStyleOptionViewItem& option, const QModelIndex& index ) const {
  Q_UNUSED(index);
  std::unique_ptr<QTextDocument> d(new QTextDocument());
  QTextOption op(d->defaultTextOption());
  op.setWrapMode(QTextOption::WordWrap);
  d->setDefaultTextOption(op);
  d->setTextWidth(option.rect.width()-PADDING_WIDTH);
  d->setDefaultFont(option.font);
  d->setHtml(option.text);
  return d;
}

bool NoteItemDelegate::eventFilter(QObject* editor, QEvent* event) {
  if(event->type()==QEvent::KeyPress) {
    QKeyEvent* e = dynamic_cast<QKeyEvent*>(event);
    if(e->key()==Qt::Key_Return) {
      QWidget* w = dynamic_cast<QWidget*>(editor);
      emit commitData(w);
      emit closeEditor(w);
      return true;
    }
  }
  return QStyledItemDelegate::eventFilter(editor, event);
}
