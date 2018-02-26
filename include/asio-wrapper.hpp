/* openASIO SDK
 * compatibility layer to allow compilation of ASIO hosts in GPL compliant way
 * asio-wrapper.hpp 
 * 
 * current version: as in original sdk only a single device can be loaded at any time
 * next version: will allow loading of several ASIO devices concurrently
 *
 * Copyright (c) 2018   Andersama <anderson.john.alexander@gmail.com>
 *                      pkv <pkv.stream@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 *
 ******************************************************************************
 * Disclaimer:                                                                *
 ******************************************************************************
 * None of the contents of this file were derived from Steinberg ASIO sdk.    *
 * The content was derived from analysis and cross-examination of asio hosts, *
 * drivers, wrapper API, forum discussions, etc                               *
 * For headers the sources for each item are given in comments.                                 *
 ******************************************************************************
 * Most of the general info on asio sdk structure was drawn from this paper:
 *
 * https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/Pa_ASIO.pdf
 *
 * Links to wrapper APIs, asio hosts or asio drivers where we found infos:
 *
 * COM interface:
 * https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/iasiothiscallresolver.cpp
 *
 * c/c++ wrappers and audio API:
 * https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp
 * https://www.codeguru.com/cpp/cpp/cpp_mfc/oop/article.php/c15891/A-Multidevice-ASIO-Output-Plugin-for-WinAMP.htm and Wasiona_source.zip
 * https://github.com/WeAreROLI/JUCE/blob/master/modules/juce_audio_devices/native/juce_win32_ASIO.cpp
 * https://github.com/sjoerdvankreel/xt-audio/blob/master/src/core/xt-asio.cpp
 *
 * java wrapper:
 * http://jsasio.sourceforge.net/
 *
 * .NET wrapper:
 * http://www.bass.radio42.com/help/html/7740a793-3341-3e90-c62c-c7bee8311772.htm
 *
 * c# wrapper:
 * https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs
 *
 * delphi wrapper:
 * http://lakeofsoft.com/vc/doc/unaASIOAPI.html
 *
 * Golang wrapper:
 * https://github.com/JamesDunne/go-asio/blob/master/asio.go
 *
 * c/c++ driver:
 * https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h
 * 
 * c/c++ hosts:
 * https://github.com/steveschow/osxwineasio/blob/master/asio.c
 * http://www.oifii.org/ns-org/nsd/ar/cp/audio_spivsthost/vsthostsrc/AsioHost.cpp
 * http://kana-soft.com/tech/sample_0004_5.htm
 * https://github.com/dechamps/FlexASIO/blob/master/flexasio.h
 *
 * Additional links given below.
 *
 ******************************************************************************
 * Legal Disclaimer:                                                          *
 * use this file in your project only if your jurisdiction allows it          *
 ******************************************************************************
 *	USA: complies with Section 103(f) of the Digital Millennium Copyright Act
 *	(DMCA) (17 USC § 1201 (f) - Reverse Engineering) which allows reverse 
 *	engineering for the purpose of interoperability between computer programs
 *	(such as information transfer between applications). Interoperability is
 *	defined in paragraph 4 of Section 103(f).
 *		Paragraph (1) of Section 103(f):
 *		(1) Notwithstanding the provisions of subsection (a)(1)(A), a person who
 *		has lawfully obtained the right to use a copy of a computer program may
 *		circumvent a technological measure that effectively controls access to a
 *		particular portion of that program for the sole purpose of identifying
 *		and analyzing those elements of the program that are necessary to
 *		achieve interoperability of an independently created computer program with
 *		other programs, and that have not previously been readily available to the
 *		 person engaging in the circumvention, to the extent any such acts of
 *		identification and analysis do not constitute infringement under this title.
 *
 *	EU: complies with EU Directive 2009/24 which superseded Article 6 of the 1991
 *	EU Computer Programs Directive.
 *	Article (15) allows reverse engineering for the purposes of interoperability :
 *		circumstances may exist when such a reproduction of the code and 
 *		translation of its form are indispensable to obtain the necessary
 *		information to achieve the interoperability of an independently created
 *		program with other programs.
 *		It has therefore to be considered that, in these limited circumstances
 *		only, performance of the acts of reproduction and translation by or on
 *		behalf of a person having a right to use a copy of the program is
 *		legitimate and compatible with fair practice and must therefore be deemed
 *		not to require the authorisation of the rightholder.
 *		An objective of this exception is to make it possible to connect all
 *		components of a computer system, including those of different
 *		manufacturers, so that they can work together.
 */
//see https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/iasiothiscallresolver.hpp#ln127

#ifndef __ASIO_H
#define __ASIO_H

/* https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/iasiothiscallresolver.cpp
 */
 typedef interface IASIO IASIO;

/* see https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2913
 * ASIOBool can take value ASIOTrue 
 * from http://lakeofsoft.com/vc/doc/unaASIOAPI.html#ASIOBool ==> type long
 * also https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln22
 */
typedef long ASIOBool;
enum {
	ASIOFalse = false,
	ASIOTrue = true
};

/* from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1955
 * type is float or double
 * is a double from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#184
 */
typedef double ASIOSampleRate;

/* from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln345
 * and ln405
 * the enum is from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln72
 * lot of details also here https://github.com/WeAreROLI/JUCE/blob/master/modules/juce_audio_devices/native/juce_win32_ASIO.cpp#ln80
 */
typedef long ASIOSampleType; 
enum {
        ASIOSTInt16MSB = 0,
        ASIOSTInt24MSB = 1, // used for 20 bits as well
        ASIOSTInt32MSB = 2,
        ASIOSTFloat32MSB = 3, // IEEE 754 32 bit float
        ASIOSTFloat64MSB = 4, // IEEE 754 64 bit double float
        ASIOSTInt32MSB16 = 8, // 32 bit data with 16 bit alignment
        ASIOSTInt32MSB18 = 9, // 32 bit data with 18 bit alignment
        ASIOSTInt32MSB20 = 10, // 32 bit data with 20 bit alignment
        ASIOSTInt32MSB24 = 11, // 32 bit data with 24 bit alignment
        ASIOSTInt16LSB = 16, // LSB == little-endian, see Juce 
        ASIOSTInt24LSB = 17, // used for 20 bits as well
        ASIOSTInt32LSB = 18,
        ASIOSTFloat32LSB = 19, // IEEE 754 32 bit float, as found on Intel x86 architecture
        ASIOSTFloat64LSB = 20, // IEEE 754 64 bit double float, as found on Intel x86 architecture
        ASIOSTInt32LSB16 = 24, // 32 bit data with 18 bit alignment
        ASIOSTInt32LSB18 = 25, // 32 bit data with 18 bit alignment
        ASIOSTInt32LSB20 = 26, // 32 bit data with 20 bit alignment
        ASIOSTInt32LSB24 = 27, // 32 bit data with 24 bit alignment
        ASIOSTDSDInt8LSB1 = 32, // DSD 1 bit data, 8 samples per byte. First sample in Least significant bit.
        ASIOSTDSDInt8MSB1 = 33, // DSD 1 bit data, 8 samples per byte. First sample in Most significant bit.
        ASIOSTDSDInt8NER8 = 40, // DSD 8 bit data, 1 sample per byte. No Endianness required.
        ASIOSTLastEntry = 41, // from http://lakeofsoft.com/vc/doc/unaASIOAPI.html#ASIOSTLastEntry
};

/* https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln13 */
typedef long ASIOError;
enum 
    {
        ASE_OK = 0,             // This value will be returned whenever the call succeeded
        ASE_SUCCESS = 0x3f4847a0,	// unique success return value for ASIOFuture calls
        ASE_NotPresent = -1000, // hardware input or output is not present or available
        ASE_HWMalfunction,      // hardware is malfunctioning (can be returned by any ASIO function)
        ASE_InvalidParameter,   // input parameter invalid
        ASE_InvalidMode,        // hardware is in a bad mode or used in a bad mode
        ASE_SPNotAdvancing,     // hardware is not running when sample position is inquired
        ASE_NoClock,            // sample clock or rate cannot be determined or is not present
        ASE_NoMemory            // not enough memory for completing the request
    };
	
/* from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln125
 * also https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln37
 */
enum ASIOMessageSelector;
enum 
    {
        kAsioSelectorSupported = 1,	// selector in <value>, returns 1L if supported,
        kAsioEngineVersion,			// returns engine (host) asio implementation version,
        kAsioResetRequest,			// request driver reset. if accepted, this
        kAsioBufferSizeChange,		// not yet supported, will currently always return 0L.
        kAsioResyncRequest,			// the driver went out of sync, such that
        kAsioLatenciesChanged, 		// the drivers latencies have changed. The engine
        kAsioSupportsTimeInfo,		// if host returns true here, it will expect the
        kAsioSupportsTimeCode,		// 
        kAsioMMCCommand,			// unused - value: number of commands, message points to mmc commands
        kAsioSupportsInputMonitor,	// kAsioSupportsXXX return 1 if host supports this
        kAsioSupportsInputGain,     // unused and undefined
        kAsioSupportsInputMeter,    // unused and undefined
        kAsioSupportsOutputGain,    // unused and undefined
        kAsioSupportsOutputMeter,   // unused and undefined
        kAsioOverload,              // driver detected an overload
    };

/* http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOClockSource.html
 * http://lakeofsoft.com/vc/doc/unaASIOAPI.ASIOClockSource.html
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln63
 */
typedef struct ASIOClockSource
{
	long index; //https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln487
	long associatedChannel; //https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln488 
	long associatedGroup; /* https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln489
	type: https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1924 */
	ASIOBool isCurrentSource; /* https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln490 
	 type is ASIOBool because can take value ASIOTrue */
	char name[32]; /* https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln491
	and array size: http://lakeofsoft.com/vc/doc/unaASIOAPI.ASIOClockSource.html */
} ASIOClockSource;

/* from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2896
and also line 57 https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln57
type inferred from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln152 , ln153, ln160 */
typedef struct ASIOSamples {
	unsigned long hi;
	unsigned long lo;
} ASIOSamples;

/* same as previous struct
 * https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2896
 */
typedef struct ASIOTimeStamp {

	unsigned long hi;
	unsigned long lo;
} ASIOTimeStamp;

/* elements names and types from:
 * https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln98
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln72
 * additional cross-refs given
 */
typedef struct ASIOChannelInfo
{
	long channel; /* also https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1123 
	and from https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln563 */
	ASIOBool isInput; /* also https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1124 (ASIOTrue ==> type ASIOBool = bool)*/
	ASIOBool isActive; /* https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln576 ==> same type as previous, so ASIOBool */
	long channelGroup; //https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln566
	ASIOSampleType type; /* https://github.com/nikkov/Win-Widget/blob/master/Driver/asiouac2.cpp#ln553 
	 and from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2412 we know the type is ASIOSampleType 
	 as well as from ln 405 BytesPerAsioSample function called at line 1655 */
	char name[32]; /* https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln4161 ; 
	array size from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln107 */
} ASIOChannelInfo;

/* https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln111
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln82
 */
typedef struct ASIOBufferInfo
{
	ASIOBool isInput; /*also https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2235 , type ASIOBool since it can be ASIOTrue
	*/
	long channelNum; /* also from: https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2240*/
	void *buffers[2]; /* also from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1653
	 ln 3299 , ln 3300 confirm length of array */
} ASIOBufferInfo;

// https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln168
// also http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOTimeCode.html
enum ASIOTimeCodeFlags;
 enum   {
        kTcValid = 1,
        kTcRunning = 1 << 1,
        kTcReverse = 1 << 2,
        kTcOnspeed = 1 << 3,
        kTcStill = 1 << 4,
        kTcSpeedValid = 1 << 8
    };


/* from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln149
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln89
 */
typedef struct ASIOTimeCode
{
	double          speed;//also from https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2943
	ASIOSamples     timeCodeSamples; /* type is from http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOTimeCode.html#timeCodeSamples()
	type found also in http://lakeofsoft.com/vc/doc/unaASIOAPI.ASIOTimeCode.html
	From https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2943
	This confirms ASIOSamples = ASIO64bit from portaudio and is 64bit.
	*/
	ASIOTimeCodeFlags   flags; //https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln154
	char future[64]; //https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln155 for array size & ln156
} ASIOTimeCode;

// https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln191
enum AsioTimeInfoFlags;
enum
    {
        kSystemTimeValid = 1,            // must always be valid
        kSamplePositionValid = 1 << 1,       // must always be valid
        kSampleRateValid = 1 << 2,
        kSpeedValid = 1 << 3,
        kSampleRateChanged = 1 << 4,
        kClockSourceChanged = 1 << 5
    };

/*
 * https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln179 and following
 * http://jsasio.sourceforge.net/com/groovemanager/spi/asio/AsioTimeInfo.html
 * https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.h#ln97
 */
typedef struct AsioTimeInfo
{
	double          speed; //https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln181
	ASIOTimeStamp   systemTime; //also  https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2933
	ASIOSamples     samplePosition; //also https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2938
	ASIOSampleRate  sampleRate; // is a double from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln184
	AsioTimeInfoFlags flags; // https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln185
	char reserved[12]; // https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln186
} AsioTimeInfo;

// from https://github.com/SjB/NAudio/blob/master/NAudio/Wave/Asio/ASIOStructures.cs#ln202
typedef struct ASIOTime
{
	long reserved[4];
	struct AsioTimeInfo     timeInfo;
	struct ASIOTimeCode     timeCode;
} ASIOTime;

/* from http://forum.cakewalk.com/m/tm.aspx?m=2232664&p=6 post from Undertow 2/22/2011
 * also http://www.bass.radio42.com/help/html/91b41d4b-9fe6-510c-502c-71156a8bbb5a.htm
 * also http://lakeofsoft.com/vc/doc/unaASIOAPI.ASIOInputMonitor.html
 */
typedef struct ASIOInputMonitor
{
    long input;        // this input was set to monitor (or off), -1: all
    long output;    // suggested output for monitoring the input (if so)
    long gain;        // suggested gain, ranging 0 - 0x7fffffffL (-inf to +12 dB)
    ASIOBool state;    // ASIOTrue => on, ASIOFalse => off
    long pan;        // suggested pan, 0 => all left, 0x7fffffff => right
} ASIOInputMonitor;

/* inferred from http://www.bass.radio42.com/help/html/715c32d7-03f0-211d-977c-8af44744b16c.htm
 * also https://www.codeguru.com/cpp/cpp/cpp_mfc/oop/article.php/c15891/A-Multidevice-ASIO-Output-Plugin-for-WinAMP.htm
 * see zip, WASIO\ASIODevice.cpp(294) can infer all following members except future
 */
typedef struct ASIOChannelControls
{
 	long channel;
	ASIOBool isInput;
	double gain;
	long meter;
	char future[32]; // from http://www.bass.radio42.com/help/html/715c32d7-03f0-211d-977c-8af44744b16c.htm
}  ASIOChannelControls;

// from http://www.bass.radio42.com/help/html/7522863c-0484-5473-a354-ef2dbb14d340.htm
typedef struct ASIOTransportParameters
{
	long command;		// One of the values in the following enum
	ASIOSamples samplePosition;
	long track; // track index
	long trackSwitches[16]; // 512 tracks on/off
	char future[64]; // up to 64 chars
} ASIOTransportParameters;

enum
{
	Start = 1,
	Stop,
	Locate,		
	PunchIn,
	PunchOut,
	ArmOn,		
	ArmOff,		
	MonitorOn,	
	MonitorOff,	
	Arm,			
	Monitor	
};

// from http://www.bass.radio42.com/help/html/8122ccee-4dd5-8034-81f0-a4ebcad47354.htm
typedef long ASIOIoFormatType;
enum
{
	FormatInvalid = -1,
	PCMFormat = 0,
	DSDFormat = 1,
};

// from http://www.bass.radio42.com/help/html/8122ccee-4dd5-8034-81f0-a4ebcad47354.htm
typedef struct ASIOIoFormat
{
	ASIOIoFormatType	FormatType;
	char			future[508];
} ASIOIoFormat;

// from http://jsasio.sourceforge.net/
typedef struct ASIODriverInfo
{
	long asioVersion;		// The asio version. Should always be 2.
	long driverVersion;		// The driver version (format is driver specific)
	char name[32];   // The driver´s name (seems limited to 32 chars)
	char errorMessage[256]; //The user message of an error that occured during ASIOInit() if any
	void *sysRef;			/* The sysRef. On windows systems this should be the application´s main window handle. Returns the main application window handle (on windows) or 0. */

} ASIODriverInfo;

/* params for asiofuture
 * from http://www.bass.radio42.com/help/html/1b592b67-9f60-4f5b-1497-7e32666485de.htm
 * (for enum and description)
 * list from https://github.com/steveschow/osxwineasio/blob/master/asio.c#ln1205
 * also http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOStaticFunctions.html
 * see also http://kana-soft.com/tech/sample_0004_5.htm  where the explanations were extracted
 * (see also // from http://lakeofsoft.com/vc/doc/unaASIOAPI.html which confirms enum starts at 1)
 */
enum
{
    kAsioEnableTimeCodeRead = 1,    // There is no parameter . 
    kAsioDisableTimeCodeRead,        // There is no parameter . 
    kAsioSetInputMonitor,            // Parameter ASIOInputMonitor structure pointer ; Return value OK = ASE_SUCCESS / Error = ASE_NotPresent
    kAsioTransport,                 // ASIOTransportParameters* in params 
    kAsioSetInputGain,               // ASIOChannelControls* in params, apply gain
    kAsioGetInputMeter,              // ASIOChannelControls* in params, fill meter
    kAsioSetOutputGain,              // ASIOChannelControls* in params, apply gain
    kAsioGetOutputMeter,            // ASIOChannelControls* in params, fill meter
    kAsioCanInputMonitor,            // There is no parameter . 
    kAsioCanTimeInfo,// There is no parameter .
    kAsioCanTimeCode,// There is no parameter .
    kAsioCanTransport,// There is no parameter .
    kAsioCanInputGain,// There is no parameter .
    kAsioCanInputMeter,// There is no parameter .
    kAsioCanOutputGain,// There is no parameter .
    kAsioCanOutputMeter,// There is no parameter .
/*
    kAsioOptionalOne = ??? from https://github.com/Mizunagi/AudioSystem/blob/e7e449b273872715fd2dcabf86ab7ca9fa458a9b/html/asio_8h.html#ln294
 */
    // from http://kana-soft.com/tech/sample_0004_5.htm and http://lakeofsoft.com/vc/doc/unaASIOAPI.html
    kAsioSetIoFormat            = 0x23111961,        /* Change the I / O format of the device. 
 - A pointer to the parameter ;
   format type ( ASIOIoFormat structure ). 
 - Return value : OK = ASE_SUCCESS / Error = ASE_NotPresent           */
    kAsioGetIoFormat            = 0x23111983,        /* Get the I / O format of the device. 
 - A pointer to the parameter; format type ( ASIOIoFormat structure ). 
 - Return value : OK = ASE_SUCCESS / Error = ASE_NotPresent */
    kAsioCanDoIoFormat            = 0x23112004,        /* Query whether the I / O format of the specified device is available or not. 
 - A pointer to the parameter; format type ( ASIOIoFormat structure ). 
 - Return value : OK = ASE_SUCCESS / Error = ASE_NotPresent */
/*    kAsioCanReportOverload = ??? from https://forum.juce.com/t/solved-kasiocanreportoverload-undeclared-identifier/25019/3
 and https://github.com/WeAreROLI/JUCE/blob/master/modules/juce_audio_devices/native/juce_win32_ASIO.cpp#ln469 
     kAsioGetInternalBufferSamples = ??? from https://github.com/Mizunagi/AudioSystem/blob/e7e449b273872715fd2dcabf86ab7ca9fa458a9b/html/asio_8h.html#ln300
*/

};

/* https://www.codeguru.com/cpp/cpp/cpp_mfc/oop/article.php/c15891/A-Multidevice-ASIO-Output-Plugin-for-WinAMP.htm
 * and Wasiona_source.zip : WASIO\ASIODevice.hpp(ln 202 and 208)
*/
typedef struct ASIOCallbacks
{
	void(*bufferSwitch) (long doubleBufferIndex, ASIOBool directProcess);
	void(*sampleRateDidChange) (ASIOSampleRate sRate);
	long(*asioMessage) (long selector, long value, void* message, double* opt);
	ASIOTime* (*bufferSwitchTimeInfo) (ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);
} ASIOCallbacks;

// from: https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/iasiothiscallresolver.cpp
interface IASIO : public IUnknown
{

	virtual ASIOBool init(void *sysHandle) = 0;
	virtual void getDriverName(char *name) = 0;	
	virtual long getDriverVersion() = 0;
	virtual void getErrorMessage(char *string) = 0;	
	virtual ASIOError start() = 0;
	virtual ASIOError stop() = 0;
	virtual ASIOError getChannels(long *numInputChannels, long *numOutputChannels) = 0;
	virtual ASIOError getLatencies(long *inputLatency, long *outputLatency) = 0;
	virtual ASIOError getBufferSize(long *minSize, long *maxSize,
		long *preferredSize, long *granularity) = 0;
	virtual ASIOError canSampleRate(ASIOSampleRate sampleRate) = 0;
	virtual ASIOError getSampleRate(ASIOSampleRate *sampleRate) = 0;
	virtual ASIOError setSampleRate(ASIOSampleRate sampleRate) = 0;
	virtual ASIOError getClockSources(ASIOClockSource *clocks, long *numSources) = 0;
	virtual ASIOError setClockSource(long reference) = 0;
	virtual ASIOError getSamplePosition(ASIOSamples *sPos, ASIOTimeStamp *tStamp) = 0;
	virtual ASIOError getChannelInfo(ASIOChannelInfo *info) = 0;
	virtual ASIOError createBuffers(ASIOBufferInfo *bufferInfos, long numChannels,
		long bufferSize, ASIOCallbacks *callbacks) = 0;
	virtual ASIOError disposeBuffers() = 0;
	virtual ASIOError controlPanel() = 0;
	virtual ASIOError future(long selector,void *opt) = 0;
	virtual ASIOError outputReady() = 0;
};

//////////////////////////////////////////////
// functions called in portaudio and RtAudio.cpp for instance
////////////////////////////////////////////////
ASIOError ASIOStart(void); // https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3324 http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOMixer.html#ASIOStart
ASIOError ASIOStop(void); // https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3363 http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOMixer.html#ASIOStop
ASIOError ASIOExit(void); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln4112 https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3324 http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOMixer.html#ASIOExit
ASIOError ASIOOutputReady(void); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln999 
ASIOError ASIOInit(ASIODriverInfo *info); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln971 http://jsasio.sourceforge.net/com/groovemanager/spi/asio/ASIOMixer.html#ASIOInit
ASIOError ASIOGetChannels(long *numInputChannels, long *numOutputChannels); //https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln982
ASIOError ASIOGetLatencies(long *inputLatency, long *outputLatency); // https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3220
ASIOError ASIOGetBufferSize(long *minSize, long *maxSize, long *preferredSize, long *granularity); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln990
ASIOError ASIOCanSampleRate(ASIOSampleRate sampleRate); // https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln2826
ASIOError ASIOGetSampleRate(ASIOSampleRate *currentRate); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1656
ASIOError ASIOSetSampleRate(ASIOSampleRate sampleRate); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1977
ASIOError ASIOGetSamplePosition(ASIOSamples *sPos, ASIOTimeStamp *tStamp); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln2888
ASIOError ASIOGetChannelInfo(ASIOChannelInfo *info); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1125
ASIOError ASIOCreateBuffers(ASIOBufferInfo *bufferInfos, long numChannels,
	long bufferSize, ASIOCallbacks *callbacks); //https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3163
ASIOError ASIODisposeBuffers(void); // https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln3122 return type guessed to e as other methods
ASIOError ASIOControlPanel(void); // https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln4101
ASIOError ASIOFuture(long selector, void *params);//https://managedbass.github.io/api/ManagedBass.Asio.AsioFuture.html

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASIO_LITTLE_ENDIAN true /* http://www.oifii.org/ns-org/nsd/ar/cp/audio_spivsthost/vsthostsrc/AsioHost.cpp
also https://forum.cockos.com/archive/index.php/t-109428.html */

// https://www.freebasic.net/forum/viewtopic.php?t=16890
#define NATIVE_INT64 0
#define IEEE754_64FLOAT 1
#define DRVERR			        -5000
#define DRVERR_INVALID_PARAM		DRVERR-1
#define DRVERR_DEVICE_ALREADY_OPEN	DRVERR-2
#define DRVERR_DEVICE_NOT_FOUND		DRVERR-3
#define MAXPATHLEN			512
#define MAXDRVNAMELEN		        128

// unused struct 
// https://www.freebasic.net/forum/viewtopic.php?t=16890
//struct asiodrvstruct
//{
//	int			drvID;
//	CLSID			clsid;
//	char			dllpath[MAXPATHLEN];
//	char			drvname[MAXDRVNAMELEN];
//	LPVOID			asiodrv; // type ????
//	struct asiodrvstruct	*next;
//};
//
//typedef struct asiodrvstruct ASIODRVSTRUCT;
//typedef ASIODRVSTRUCT	*LPASIODRVSTRUCT; // https://www.freebasic.net/forum/viewtopic.php?t=16890

// we use tinyasio implementation (GPL v3 or later)
// https://github.com/eiz/SynchronousAudioRouter/blob/master/SarAsio/tinyasio.cpp#ln23

struct AsioDriver
{
	std::string name;
	std::string clsid;
	LPVOID     Device;
//	HRESULT open(IASIO **ppAsio);
};

std::vector<AsioDriver> InstalledAsioDrivers();

										  /* http://lakeofsoft.com/vc/doc/unaASIOAPI.unaAsioDriverList.html
										  */
class AsioDriverList {
public:
	AsioDriverList();
	~AsioDriverList();

	LONG asioOpenDriver(int, VOID **); // http://www.fmod.org/questions/question/forum-13594/
	LONG asioCloseDriver(int); // arg is driver index
	LONG asioGetNumDev(VOID); /* https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#1252
							  type inferred from http://lakeofsoft.com/vc/doc/unaASIOAPI.unaAsioDriverList.html and
							  https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1173 + ln 1252
							  also https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln2749 */
	ASIOError asioGetDriverName(int, char *, int); /* type is ASIOError  https://github.com/thestk/rtaudio/blob/master/RtAudio.cpp#ln2782
												   ASIOError = long from http://lakeofsoft.com/vc/doc/unaASIOAPI.html#ASIOError ;
												   type of DRVERR_DEVICE_NOT_FOUND https://www.freebasic.net/forum/viewtopic.php?t=16890 so type is LONG for next two entries  */
	LONG asioGetDriverPath(int, char *, int); // type of DRVERR_DEVICE_NOT_FOUND https://www.freebasic.net/forum/viewtopic.php?t=16890
	LONG asioGetDriverCLSID(int, CLSID *); // type of DRVERR_DEVICE_NOT_FOUND https://www.freebasic.net/forum/viewtopic.php?t=16890
//	LPASIODRVSTRUCT	lpdrvlist; // type from https://www.freebasic.net/forum/viewtopic.php?t=16890 not implemented
	int numdrv; // number of devices
};


/* http://jsasio.sourceforge.net/com/groovemanager/spi/asio/AsioDrivers.html
* https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln1213
*/
class AsioDrivers : public AsioDriverList
{
public:
	AsioDrivers();
	~AsioDrivers();

	bool getCurrentDriverName(char *name); // http://d.hatena.ne.jp/heisseswasser/touch/searchdiary?word=*%5BASIO%5D
	long getDriverNames(char **names, long maxDrivers);/* https://svn.grrrr.org/ext/tags/pd/before_convergence_to_pd_cvs/src/s_audio_asio.cpp
													   in asio_getdevs function
													   also https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln338 */
	bool loadDriver(char *name); //https://app.assembla.com/spaces/portaudio/git/source/master/src/hostapi/asio/pa_asio.cpp#ln961 & ln4073
	void removeCurrentDriver();
	long getCurrentDriverIndex() { return curIndex; } // https://github.com/mhroth/jasiohost/blob/master/src/com/synthbot/jasiohost/JAsioHost.cpp#ln314
	long curIndex;
};
#endif