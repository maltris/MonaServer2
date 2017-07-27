/*
This file is a part of MonaSolutions Copyright 2017
mathieu.poux[a]gmail.com
jammetthomas[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

*/

#pragma once

#include "Mona/Mona.h"
#include "Mona/MediaWriter.h"

namespace Mona {


struct MP4Writer : MediaWriter, virtual Object {
	// https://w3c.github.io/media-source/isobmff-byte-stream-format.html
	// http://l.web.umkc.edu/lizhu/teaching/2016sp.video-communication/ref/mp4.pdf
	// https://www.adobe.com/content/dam/Adobe/en/devnet/flv/pdfs/video_file_format_spec_v10.pdf
	// https://developer.apple.com/library/content/documentation/QuickTime/QTFF/QTFFChap2/qtff2.html

	void beginMedia(const OnWrite& onWrite);
	void writeProperties(const Media::Properties& properties, const OnWrite& onWrite);
	void writeAudio(UInt8 track, const Media::Audio::Tag& tag, const Packet& packet, const OnWrite& onWrite);
	void writeVideo(UInt8 track, const Media::Video::Tag& tag, const Packet& packet, const OnWrite& onWrite);
	void endMedia(const OnWrite& onWrite);

private:
	void flush(const OnWrite& onWrite);

	struct Audios : virtual Object, std::deque<Media::Audio> {
		Audios() : rate(0), timeRef(0) {}
		Audios(Audios&& audios) : config(std::move(audios.config)), rate(audios.rate), timeRef(audios.timeRef), std::deque<Media::Audio>(std::move(audios)) {}
		Packet  config;
		UInt32  rate;
		UInt32  timeRef;
		UInt32  sizeTraf;
	};
	struct Videos : virtual Object, std::deque<Media::Video> {
		Videos() : timeRef(0) {}
		Videos(Videos&& videos) : sps(std::move(videos.sps)), pps(std::move(videos.pps)), timeRef(videos.timeRef), std::deque<Media::Video>(std::move(videos)) {}
		Packet  sps;
		Packet	pps;
		UInt32  timeRef;
		UInt32  sizeTraf;
	};

	UInt64						_offset;
	bool						_loading;
	std::vector<Audios>			_audios;
	std::vector<Videos>			_videos;
	bool						_audioOutOfRange;
	bool						_videoOutOfRange;
	UInt32						_sequence;
	Media::Base*				_pAVFront;
	Media::Base*				_pAVBack;
};



} // namespace Mona
