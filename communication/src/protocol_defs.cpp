/*
 * Copyright (c) 2016 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "mbedtls_config.h"

#include "protocol_defs.h"

namespace particle {

namespace protocol {

#if PLATFORM_GEN >= 3
static_assert(MAX_EVENT_MESSAGE_SIZE <= PROTOCOL_BUFFER_SIZE, "MAX_EVENT_MESSAGE_SIZE is too large");
static_assert(MAX_FUNCTION_CALL_MESSAGE_SIZE <= PROTOCOL_BUFFER_SIZE, "MAX_FUNCTION_CALL_MESSAGE_SIZE is too large");
static_assert(MAX_VARIABLE_VALUE_MESSAGE_SIZE <= PROTOCOL_BUFFER_SIZE, "MAX_VARIABLE_VALUE_MESSAGE_SIZE is too large");
#endif // PLATFORM_GEN >= 3

system_error_t toSystemError(ProtocolError error) {
    switch (error) {
    case NO_ERROR:
        return SYSTEM_ERROR_NONE;
    case PING_TIMEOUT:
    case MESSAGE_TIMEOUT:
        return SYSTEM_ERROR_TIMEOUT;
    case IO_ERROR:
    case IO_ERROR_FORWARD_MESSAGE_CHANNEL:
    case IO_ERROR_SET_DATA_MAX_EXCEEDED:
    case IO_ERROR_PARSING_SERVER_PUBLIC_KEY:
    case IO_ERROR_GENERIC_ESTABLISH:
    case IO_ERROR_GENERIC_RECEIVE:
    case IO_ERROR_GENERIC_SEND:
    case IO_ERROR_GENERIC_MBEDTLS_SSL_WRITE:
    case IO_ERROR_DISCARD_SESSION:
    case IO_ERROR_LIGHTSSL_BLOCKING_SEND:
    case IO_ERROR_LIGHTSSL_BLOCKING_RECEIVE:
    case IO_ERROR_LIGHTSSL_RECEIVE:
    case IO_ERROR_LIGHTSSL_HANDSHAKE_NONCE:
    case IO_ERROR_LIGHTSSL_HANDSHAKE_RECV_KEY:
    case IO_ERROR_SOCKET_SEND_FAILED:
    case IO_ERROR_SOCKET_RECV_FAILED:
        return SYSTEM_ERROR_IO;
    case INVALID_STATE:
        return SYSTEM_ERROR_INVALID_STATE;
    case AUTHENTICATION_ERROR:
        return SYSTEM_ERROR_NOT_ALLOWED;
    case BANDWIDTH_EXCEEDED:
        return SYSTEM_ERROR_LIMIT_EXCEEDED;
    case INSUFFICIENT_STORAGE:
        return SYSTEM_ERROR_TOO_LARGE;
    case NOT_IMPLEMENTED:
        return SYSTEM_ERROR_NOT_SUPPORTED;
    case NOT_FOUND:
        return SYSTEM_ERROR_NOT_FOUND;
    case NO_MEMORY:
        return SYSTEM_ERROR_NO_MEMORY;
    case INTERNAL:
        return SYSTEM_ERROR_INTERNAL;
    case OTA_UPDATE_ERROR:
        return SYSTEM_ERROR_OTA;
    case IO_ERROR_REMOTE_END_CLOSED:
        return SYSTEM_ERROR_END_OF_STREAM;
    default:
        return SYSTEM_ERROR_PROTOCOL; // Generic protocol error
    }
}

} // namespace protocol

} // namespace particle
