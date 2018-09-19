/*
   Copyright (C) 2017 Alexandr Akulich <akulichalexander@gmail.com>

   This file is a part of TelegramQt library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#ifndef TELEGRAMDEBUG_P_HPP
#define TELEGRAMDEBUG_P_HPP

#include "Debug.hpp"
#include "TLNumbers.hpp"
#include "TLValues.hpp"

QDebug operator<<(QDebug d, const TLValue &v);

template <int Size>
QDebug operator<<(QDebug d, const TLNumber<Size> &n);

namespace Telegram {

namespace MTProto {

struct FullMessageHeader;
struct IgnoredMessageNotification;

} // MTProto namespace

namespace Debug {

class Spacer
{
public:
    Spacer();
    ~Spacer();

    QString innerSpaces();
    QString outerSpaces();

private:
    static int m_spacing;
    static const int m_step = 4;
    bool m_hasInnerCalls = false;
};

} // Debug

template <typename T>
QString toHex(T number)
{
    return QStringLiteral("%1").arg(number, sizeof(number) * 2, 0x10, QLatin1Char('0'));
}

} // Telegram

TELEGRAMQT_EXPORT QDebug operator<<(QDebug d, const Telegram::MTProto::FullMessageHeader &header);
TELEGRAMQT_EXPORT QDebug operator<<(QDebug d, const Telegram::MTProto::IgnoredMessageNotification &notification);

#endif // TELEGRAMDEBUG_P_HPP
