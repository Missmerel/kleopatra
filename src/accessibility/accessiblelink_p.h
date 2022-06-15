/*
    accessibility/accessiblelink_p.h

    This file is part of Kleopatra, the KDE keymanager
    SPDX-FileCopyrightText: 2022 g10 Code GmbH
    SPDX-FileContributor: Ingo Klöcker <dev@ingo-kloecker.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QAccessible>
#include <QPointer>

class QWidget;

namespace Kleo
{
class AnchorProvider;

class AccessibleLink: public QAccessibleInterface
{
public:
    AccessibleLink(QWidget *label, int index);
    ~AccessibleLink() override;

    bool isValid() const override;
    QObject *object() const override;
    QWindow *window() const override;

    QAccessibleInterface *childAt(int x, int y) const override;

    QAccessibleInterface *parent() const override;
    QAccessibleInterface *child(int index) const override;
    int childCount() const override;
    int indexOfChild(const QAccessibleInterface * child) const override;

    QString text(QAccessible::Text t) const override;
    void setText(QAccessible::Text t, const QString & text) override;
    QRect rect() const override;
    QAccessible::Role role() const override;
    QAccessible::State state() const override;

    int index() const;

private:
    AnchorProvider *anchorProvider() const;

    QPointer<QWidget> mLabel;
    int mIndex;
};

}
