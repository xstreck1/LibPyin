using System.Runtime.InteropServices;
using System;

#if UNITY_EDITOR
using UnityEngine;
#endif

// This code demonstrates using the C interface to LibPyin from C# or Unity3D

public class LoadLibrary  {
    [StructLayout(LayoutKind.Sequential)]
    unsafe public struct pyinc_pitch_range
    {
        public float* begin;
        public float* end;
    }

    [DllImport("PyinCpp")]
    static extern void pyinc_init(int sample_rate, int block_size, int step_size);

    [DllImport("PyinCpp")]
    unsafe static extern pyinc_pitch_range pyinc_feed(float * data, int size);

    [DllImport("PyinCpp")]
    unsafe static extern void pyinc_clear();

    // Use this for initialization
    unsafe static public void use () {
        int SAMPLE_RATE = 44100;
        int SAMPLE_COUNT = 10000;
        int BLOCK_SIZE = 2048;
        int STEP_SIZE = 512;

        // Prepare objects
        pyinc_init(SAMPLE_RATE, BLOCK_SIZE, STEP_SIZE);
        float* ptr = stackalloc float[SAMPLE_COUNT];


        // Generate a 440 herz sine wave
        double freq = 440;
        double angle_speed = 2 * Math.PI * (freq / SAMPLE_RATE);
        for (int i = 0; i < SAMPLE_COUNT; i++)
            ptr[i] = (float) Math.Sin(angle_speed * i);

        // Mine pitches
        pyinc_pitch_range pitches = pyinc_feed(ptr, SAMPLE_COUNT);

        // Go through and print the pitches
        float* res_ptr = pitches.begin;
        while (res_ptr != pitches.end)
        {
#if UNITY_EDITOR
            Debug.Log(*res_ptr);
#else
            Console.Write(*res_ptr);  
#endif
            res_ptr++;
        }

        // Release the data
        pyinc_clear();
    }
}
