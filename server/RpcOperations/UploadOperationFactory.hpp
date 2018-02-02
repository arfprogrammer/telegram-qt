/*
   Copyright (C) 2018 Alexander Akulich <akulichalexander@gmail.com>

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

#ifndef UPLOAD_OPERATION_FACTORY_HPP
#define UPLOAD_OPERATION_FACTORY_HPP

#include "RpcOperationFactory.hpp"
#include "ServerRpcOperation.hpp"

#include <QObject>

namespace Telegram {

namespace Server {

class UploadRpcOperation : public RpcOperation
{
    Q_OBJECT
    Q_DISABLE_COPY(UploadRpcOperation)
public:
    explicit UploadRpcOperation(RpcLayer *rpcLayer) : RpcOperation(rpcLayer) { }

    // Generated process methods
    bool processGetFile(RpcProcessingContext &context);
    bool processSaveBigFilePart(RpcProcessingContext &context);
    bool processSaveFilePart(RpcProcessingContext &context);
    // End of generated process methods

    // Generated run methods
    void runGetFile();
    void runSaveBigFilePart();
    void runSaveFilePart();
    // End of generated run methods

    void start() override { callMember<>(this, m_runMethod); }

    using RunMethod = void (UploadRpcOperation::*)();
    using ProcessingMethod = bool (UploadRpcOperation::*)(RpcProcessingContext &);
    static ProcessingMethod getMethodForRpcFunction(TLValue function);
protected:
    void setRunMethod(RunMethod method);

    RunMethod m_runMethod = nullptr;

    // Generated RPC members
    TLFunctions::TLUploadGetFile m_getFile;
    TLFunctions::TLUploadSaveBigFilePart m_saveBigFilePart;
    TLFunctions::TLUploadSaveFilePart m_saveFilePart;
    // End of generated RPC members
};

class UploadOperationFactory : public RpcOperationFactory
{
public:
    RpcOperation *processRpcCall(RpcLayer *layer, RpcProcessingContext &context) override;
};

} // Server

} // Telegram

#endif // UPLOAD_OPERATION_FACTORY_HPP