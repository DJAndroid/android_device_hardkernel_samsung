/*
 * Copyright@ Samsung Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*!
 * \file      ExynosMutex.cpp
 * \brief     source file for ExynosMutex
 * \author    Sangwoo, Park(sw5771.park@samsung.com)
 * \date      2011/06/15
 *
 * <b>Revision History: </b>
 * - 2010/06/15 : Sangwoo, Park(sw5771.park@samsung.com) \n
 *   Initial version
 *
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "ExynosMutex"
#include <utils/Log.h>

#include <utils/threads.h>
using namespace android;

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "ExynosMutex.h"

//#define EXYNOS_MUTEX_DEBUG

ExynosMutex::ExynosMutex(
    int type,
    char* name)
{
    int androidMutexType = 0;
    m_mutex = NULL;
    m_type = TYPE_BASE;

    switch (type) {
    case TYPE_PRIVATE:
        androidMutexType = Mutex::PRIVATE;
        break;
    case TYPE_SHARED:
        androidMutexType = Mutex::SHARED;
        break;
    default:
        LOGE("%s::unmatched type(%d) fail", __func__, type);
        break;
    }

    m_mutex = new Mutex(androidMutexType, name);
    if (m_mutex == NULL) {
        LOGE("%s::Mutex create fail", __func__);
    }

    m_type = type;
    strcpy(m_name, name);
}

ExynosMutex::~ExynosMutex()
{
    if (m_mutex)
        delete ((Mutex *)m_mutex);
    m_mutex = NULL;
}

bool ExynosMutex::lock(
    void)
{
#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'lock() start", __func__, m_name);
#endif

    if (m_mutex == NULL) {
        LOGE("%s::Mutex create fail", __func__);
        return false;
    }

    if (((Mutex *)m_mutex)->lock() != 0) {
        LOGE("%s::m_core->lock() fail", __func__);
        return false;
    }

#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'lock() end", __func__, m_name);
#endif

    return true;
}

bool ExynosMutex::unLock(
    void)
{
#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'unlock() start", __func__, m_name);
#endif

    if (m_mutex == NULL) {
        LOGE("%s::Mutex create fail", __func__);
        return false;
    }

    ((Mutex *)m_mutex)->unlock();

#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'unlock() end", __func__, m_name);
#endif

    return true;
}

bool ExynosMutex::tryLock(
    void)
{
    int ret = 0;

#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'trylock() start", __func__, m_name);
#endif

    if (m_mutex == NULL) {
        LOGE("%s::Mutex create fail", __func__);
        return false;
    }

    ret = ((Mutex *)m_mutex)->tryLock();

#ifdef EXYNOS_MUTEX_DEBUG
    LOGD("%s::%s'trylock() end", __func__, m_name);
#endif

    return (ret == 0) ? true : false;
}

int ExynosMutex::getType(
    void)
{
    return m_type;
}

int ExynosMutex::getCreatedStatus(
    void)
{
    if (m_mutex == NULL)
        return STATUS_NOT_CREATED;
    else
        return STATUS_CREATED;
}

void *exynos_mutex_create(
    int type,
    char *name)
{
    ExynosMutex *mutex = new ExynosMutex(type, name);

    return (void*)mutex;
}

bool exynos_mutex_destroy(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    delete (ExynosMutex *)handle;

    return true;
}

bool exynos_mutex_lock(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    return ((ExynosMutex *)handle)->lock();

}

bool exynos_mutex_unlock(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    return ((ExynosMutex *)handle)->unLock();

}

bool exynos_mutex_trylock(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    return ((ExynosMutex *)handle)->tryLock();

}

int exynos_mutex_get_type(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    return ((ExynosMutex *)handle)->getType();
}

int exynos_mutex_get_created_status(
    void *handle)
{
    if (handle == NULL) {
        LOGE("%s::handle is null", __func__);
        return false;
    }

    return ((ExynosMutex *)handle)->getCreatedStatus();
}

