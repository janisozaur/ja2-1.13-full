<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML><HEAD>
<META http-equiv=Content-Type content="text/html; charset=windows-1252"></HEAD>
<BODY><PRE>using System;
using System.Drawing;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;


namespace Ja2Lib.Sti
{
    /// &lt;summary&gt;
    /// Loads STI images and converts them into System.Drawing.Image objects.
    /// &lt;/summary&gt;
    public class Sti : IDisposable
    {
        Stream stream;
        BinaryReader br;

        StiHeader header;
        byte[] palette;

        Image[] frame;
        StiSubImageHeader[] frameHeader;


        /// &lt;summary&gt;
        /// Opens an STI file.
        /// &lt;/summary&gt;
        /// &lt;param name="filename"&gt;&lt;/param&gt;
        public Sti(string filename)
            : this(new FileStream(filename, FileMode.Open, FileAccess.Read))
        {
        }


        /// &lt;summary&gt;
        /// Opens an STI image from a stream.
        /// &lt;/summary&gt;
        /// &lt;param name="stream"&gt;Stream containing the STI image.&lt;/param&gt;
        public Sti(Stream stream)
        {
            this.stream = stream;
            br = new BinaryReader(stream, Encoding.Default);
            ParseHeader();
        }


        struct StiHeader
        {
            public const int TotalHeaderSize = 64;
            public const int BytesPerPixel = 3;

            public const int IdLength = 4;
            public UInt32 originalSize;
            public UInt32 storedSize;
            public UInt32 transparentValue;
            public UInt32 flags;
            public UInt16 height;
            public UInt16 width;

            
            public UInt32 numberOfColors;
            public UInt16 numberOfSubImages;
            public byte redDepth;
            public byte greenDepth;
            public byte blueDepth;

            public byte depth;
            public UInt32 appDataSize;
        }


        struct StiSubImageHeader
        {
            public const int TotalSubImageHeaderSize = 16;

            public UInt32 dataOffset;
            public UInt32 dataLength;
            public Int16 offsetX;
            public Int16 offsetY;
            public UInt16 height;
            public UInt16 width;
        }


        private void ParseHeader()
        {
            stream.Position = 0;

            Encoding enc = Encoding.ASCII;
            string idstring = enc.GetString(br.ReadBytes(StiHeader.IdLength));
            if (idstring != "STCI")
            {
                throw new Exception("Image format differs from STI format.");
            }

            header.originalSize = br.ReadUInt32();
            header.storedSize = br.ReadUInt32();
            header.transparentValue = br.ReadUInt32();
            header.flags = br.ReadUInt32();
            header.height = br.ReadUInt16();
            header.width = br.ReadUInt16();

            header.numberOfColors = br.ReadUInt32();
            header.numberOfSubImages = br.ReadUInt16();
            header.redDepth = br.ReadByte();
            header.greenDepth = br.ReadByte();
            header.blueDepth = br.ReadByte();

            header.depth = br.ReadByte();
            header.appDataSize = br.ReadUInt32();

            // skip padding bytes
            br.BaseStream.Position = 64;

            // load palette
            palette = br.ReadBytes((int)header.numberOfColors * StiHeader.BytesPerPixel);

            frame = new Image[header.numberOfSubImages];
            frameHeader = new StiSubImageHeader[header.numberOfSubImages];

            for (int i = 0; i &lt; header.numberOfSubImages; i++)
            {
                frameHeader[i].dataOffset = br.ReadUInt32();
                frameHeader[i].dataLength = br.ReadUInt32();
                frameHeader[i].offsetX = br.ReadInt16();
                frameHeader[i].offsetY = br.ReadInt16();
                frameHeader[i].height = br.ReadUInt16();
                frameHeader[i].width = br.ReadUInt16();
            }
        }


        /// &lt;summary&gt;
        /// Provides access to single frames within the STI image.
        /// &lt;/summary&gt;
        /// &lt;param name="index"&gt;Specifies the position of the frame.&lt;/param&gt;
        /// &lt;returns&gt;An image object of the STI.&lt;/returns&gt;
        public Image this[int index]
        {
            get
            {
                //return frame[index];
                return LoadFrame(index);
            }
        }


        /// &lt;summary&gt;
        /// Loads and decodes a single frame of the STI image.
        /// &lt;/summary&gt;
        /// &lt;param name="index"&gt;Specifies the position of the frame.&lt;/param&gt;
        /// &lt;returns&gt;An image object of the STI.&lt;/returns&gt;
        public Image LoadFrame(int index)
        {
            if (frame[index] == null)
            {
                // load the data first
                DecodeData(index);
            }
                
            return frame[index];
        }


        /// &lt;summary&gt;
        /// Loads and decodes all frames within the STI image.
        /// &lt;/summary&gt;
        public void Load()
        {
            // load all subframes
            for (int i = 0; i &lt; header.numberOfSubImages; i++)
            {
                LoadFrame(i);
            }
        }


        /// &lt;summary&gt;
        /// Gets the number of frames within the STI image.
        /// &lt;/summary&gt;
        public int NumberOfSubImages
        {
            get { return header.numberOfSubImages; }
        }


        private void DecodeData(int index)
        {
            using (MemoryStream decodedData
                = new MemoryStream(frameHeader[index].width * frameHeader[index].height * 4))    // 32 bbp
            {
                BinaryWriter decodedWriter = new BinaryWriter(decodedData);
                BinaryReader encodedReader = br;

                // make sure stream is at correct position
                stream.Position = StiHeader.TotalHeaderSize
                    + (header.numberOfColors * StiHeader.BytesPerPixel)
                    + (header.numberOfSubImages * StiSubImageHeader.TotalSubImageHeaderSize)
                    + frameHeader[index].dataOffset;

                
                // The following algorithm has been developed on basis of
                // information from the LOTB forums. This is not identical
                // to the solution in the original JA2 sourcecode.
                
                int byteCount = 0;
                                
                while (byteCount &lt; frameHeader[index].dataLength)
                {
                    byte currentByte = encodedReader.ReadByte();
                    ++byteCount;

                    // we always start with a control byte
                    if ((currentByte &amp; 0x80) &gt; 0)
                    {
                        // we have some transparent pixels
                        for (int i = 0; i &lt; (currentByte &amp; 0x7f); ++i)
                        {
                            
                            decodedWriter.Write(palette[0 + 2]);
                            decodedWriter.Write(palette[0 + 1]);
                            decodedWriter.Write(palette[0]);
                            decodedWriter.Write((byte)255);
                        }
                    }
                    else
                    {
                        for (int i = 0; i &lt; currentByte; ++i)
                        {
                            // we have some non-transparent pixels
                            byte colorIndex = encodedReader.ReadByte();

                            decodedWriter.Write(palette[colorIndex * 3 + 2]);
                            decodedWriter.Write(palette[colorIndex * 3 + 1]);
                            decodedWriter.Write(palette[colorIndex * 3]);
                            decodedWriter.Write((byte)255);
                            
                            ++byteCount;
                        }
                    }
                }

                // this is not the fastest way, but perfectly safe                
                Bitmap bmp = new Bitmap(frameHeader[index].width, frameHeader[index].height,
                    PixelFormat.Format32bppArgb);

                BitmapData bmpData = bmp.LockBits(
                    new Rectangle(0, 0, frameHeader[index].width, frameHeader[index].height),
                    System.Drawing.Imaging.ImageLockMode.WriteOnly,
                    System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                Marshal.Copy(decodedData.ToArray(), 0, bmpData.Scan0, (int)decodedData.Length);

                bmp.UnlockBits(bmpData);

                frame[index] = bmp;
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            if (stream != null)
                stream.Dispose();

            foreach (Image i in frame)
            {
                if (i != null)
                    i.Dispose();
            }
        }

        #endregion
    }
}
</PRE></BODY></HTML>
