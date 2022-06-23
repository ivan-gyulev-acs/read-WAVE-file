#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <windows.h>
#include <string>

struct WaveFile
{
    void print()
    {
        std::cout << "ChunkID:\t\t[";
        for(int a = 0; a < 4; a++) std::cout << ChunkID[a];
        std::cout                                                    << "]\n";
        std::cout << "ChunkSize:\t\t["          << ChunkSize         << "]\n";
        std::cout << "FileFormat:\t\t[";
        for(int a = 0; a < 4; a++) std::cout << FileFormat[a];
        std::cout                                                    << "]\n";
        std::cout << "Subchunk1_ID:\t\t[";
        for(int a = 0; a < 4; a++) std::cout << Subchunk1_ID[a];
        std::cout                                                    << "]\n";
        std::cout << "Subchunk1_Size:\t\t["     << Subchunk1_Size    << "]\n";
        std::cout << "AudioFormat:\t\t["        << AudioFormat       << "]\n";
        std::cout << "Channels:\t\t["           << Channels          << "]\n";
        std::cout << "SampleRate:\t\t["         << SampleRate        << "]\n";
        std::cout << "ByteRate:\t\t["           << ByteRate          << "]\n";
        std::cout << "BlockAlign:\t\t["         << BlockAlign        << "]\n";
        std::cout << "BitsPerSample:\t\t["      << BitsPerSample     << "]\n";
        std::cout << "Subchunk2_ID:\t\t[";
        for(int a = 0; a < 4; a++) std::cout << Subchunk2_ID[a];
        std::cout                                                    << "]\n";
        std::cout << "Subchunk2_Size:\t\t["     << Subchunk2_Size    << "]\n";
    }
    char            ChunkID[4];
    unsigned int    ChunkSize;
    char            FileFormat[4];
    char            Subchunk1_ID[4];
    unsigned int    Subchunk1_Size;
    unsigned short  AudioFormat;
    unsigned short  Channels;
    unsigned int    SampleRate;
    unsigned int    ByteRate;
    unsigned short  BlockAlign;
    unsigned short  BitsPerSample;
    char            Subchunk2_ID[4];
    unsigned int    Subchunk2_Size;
};

int main(int argc, char* argv[])
{
    std::string fileLocation;
    int sampleCompression;
    try
    {
        fileLocation = argv[1];
        sampleCompression = stoi(std::string(argv[2]));
        1/stoi(std::string(argv[2]));
    }
    catch(...)
    {
        return 1;
    }

    WaveFile wav;

    std::ifstream fileReader;
    fileReader.open(fileLocation, std::ios::binary);
    if(!fileReader.is_open()) return 1;

    fileReader.read(wav.ChunkID, 4);
    fileReader.read((char*)&wav.ChunkSize, 4);
    fileReader.read(wav.FileFormat, 4);
    fileReader.read(wav.Subchunk1_ID, 4);
    fileReader.read((char*)&wav.Subchunk1_Size, 4);
    fileReader.read((char*)&wav.AudioFormat, 2);
    fileReader.read((char*)&wav.Channels, 2);
    fileReader.read((char*)&wav.SampleRate, 4);
    fileReader.read((char*)&wav.ByteRate, 4);
    fileReader.read((char*)&wav.BlockAlign, 2);
    fileReader.read((char*)&wav.BitsPerSample, 2);
    fileReader.read(wav.Subchunk2_ID, 4);
    fileReader.read((char*)&wav.Subchunk2_Size, 2);
    fileReader.seekg(sizeof(char) * 2);

    std::ofstream fileWriter;
    std::ofstream fileWriter2;
    fileWriter.open("Channel1.csv");
    fileWriter2.open("Channel2.csv");

    int sampleCount = 0;
    for(short a = 0; !fileReader.eof(); sampleCount++)
    {
        if(sampleCount % sampleCompression != 0) 
        {
            fileReader.read((char*)&a, 2);
            fileReader.read((char*)&a, 2);
            continue;
        }
        fileReader.read((char*)&a, 2);
        fileWriter << (1.0f * sampleCount / wav.SampleRate) << ',' << a << '\n';
        fileReader.read((char*)&a, 2);
        fileWriter2 << (1.0f * sampleCount / wav.SampleRate) << ',' << a << '\n';
    }
    fileReader.close();
    fileWriter.close();
    fileWriter2.close();
    wav.print();
    std::cout << "Sample Compresion:\t[" << sampleCompression << "]\n";
    return 0;
}