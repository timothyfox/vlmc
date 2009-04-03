/*****************************************************************************
 * Media.h: Class for media handling
 *****************************************************************************
 * Copyright (C) 2008-2009 the VLMC team
 *
 * Authors: Hugo Beauzee-Luyssen <hugo@vlmc.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QImage>
#include <QThread>

#include "VLCMedia.h"
#include "VLCInstance.h"
#include "VLCMediaPlayer.h"

#include "Image.h"

class       Media : private QThread
{
    Q_OBJECT
public:
    Media( const QString& mrl );
    ~Media();

    static void             lock( LibVLCpp::Media::DataCtx* dataCtx, void **pp_ret );
    static void             unlock( LibVLCpp::Media::DataCtx* dataCtx );

    QImage*                 takeSnapshot( unsigned int width, unsigned int heigth );

    /**
      * Ask libvlc if the media is currently playing
      */
    bool                    isPlaying();
    /**
      * Ask libvlc if the media can be seeked
      */
    bool                    isSeekable();
    /**
      * Can be used to know if the Media is fully usable (IE. can be seeked, vmem can be used, etc...)
      */
    bool                    isReady();
    /**
      * Return the length (duration) of a Media
      */
    qint64                  getLength();
    /**
      * Returns the last rendered frame
      */
    QImage&                 getImage();
    /**
      * Start the playback.
      * This need to be called at least once in order to prepare the media. if not, the media can't be seeked or anything else.
      * When pre-launching is completed, a "mediaReady" signal will be fired.
      */
    void                    play();
    void                    setDrawable( WId handle );

private:
    virtual void            run();

private:
    LibVLCpp::Media*            m_vlcMedia;
    LibVLCpp::MediaPlayer*      m_vlcMediaPlayer;
    LibVLCpp::Instance*         m_instance;
    QString                     m_mrl;
    QImage*                     m_snapshot;
    uchar*                      m_pixelBuffer;
    QImage*                     m_image;
    bool                        m_isMediaInitialized;

private slots:
    void                        playSlot();

signals:
    void                        mediaReady();
};

#endif // MEDIA_H