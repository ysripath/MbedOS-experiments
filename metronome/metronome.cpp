#include "metronome.hpp"

/*private:
    bool m_timing;
    Timer m_timer;

    // Insert new samples at the end of the array, removing the oldest
    size_t m_beats[beat_samples];
    size_t m_beat_count;*/

// Call when entering "learn" mode
int utilCount = 0;


void metronome::start_timing()
{
    printf("On start timing\n");
    //m_timer.start();
    m_timing = true;
    utilSampleCount = 0;
    //utilTimerBuffer = m_timer.read();
    //utilTimerBuffer = m_timer.read_ms();
    //printf("utilTimerBuffer %f\n",utilTimerBuffer);
}

// Call when leaving "learn" mode
void metronome::stop_timing()
{
    printf("On stop timinng %d\n",utilSampleCount);
    m_timer.stop();
    m_timer.reset();
    m_timing = false;
    // check for valid samples
   // if (utilSampleCount != beat_samples)
     //   m_beat_count = 0;
    //else
    //{
        int sum = 0;
        for (int i = 0; i < beat_samples; i++)
        {
            printf("beat sample %d = %u \n",i,m_beats[i]);
            sum += m_beats[i];
        }
        
        if (utilCount > beat_samples)
        {
            int avg = sum/beat_samples;
            m_beat_count = (size_t)(60/avg);
            printf("Beat count is %d\n",60/m_beat_count);
        }
        else
        {
            printf("Need atlease 4 taps to set the BPM\n");
            m_beat_count = 0;
        }
        utilCount = 0;
    //}
}

// utility function for calculating difference in time
size_t utilDiff(float time1, float time2)
{
    // assuming time 1 and time 2 are already in seconds format
    size_t temp = (size_t)(time2 - time1);
   // printf("utilDiff of %f - %f is %u\n",time2,time1,temp);
    return temp;
}

// Should only record the current time when timing
// Insert the time at the next free position of m_beats
void metronome::tap()
{
    printf("on metronome tap\n");
    if (utilCount == 0)
    {
        m_timer.start();
        utilTimerBuffer = m_timer.read();    
        printf("utilTimerBuffer %f\n",utilTimerBuffer);
        utilCount++;
        return;
    }
    
    
    utilCount++;
    
        printf("\n timing %d\n",utilSampleCount);
        if (utilSampleCount < beat_samples)
        {
            float tempTimerRead = m_timer.read();
            printf("Time read %f \n",m_timer.read());
            //float tempTimerRead = m_timer.read_ms();
            //printf("Time read %f \n",m_timer.read_ms());
            m_beats[utilSampleCount++] = utilDiff(utilTimerBuffer,tempTimerRead);
            printf ("Sample count is %d\n",utilSampleCount);
            utilTimerBuffer = tempTimerRead;
            printf("Time diff is %u\n",m_beats[utilSampleCount - 1]);
        }
        else
        {
            utilSampleCount = 0;
            printf("Time read %f \n",m_timer.read());
            float tempTimerRead = m_timer.read();
            
            //printf("Time read %f \n",m_timer.read_ms());
            //float tempTimerRead = m_timer.read_ms();
            m_beats[utilSampleCount++] = utilDiff(utilTimerBuffer,tempTimerRead);
            utilTimerBuffer = tempTimerRead;
            printf("Time diff is %u\n",m_beats[utilSampleCount - 1]);
            
           //printf("Timing is false %d\n",utilSampleCount);
        }
   
}


    // Calculate the BPM from the deltas between m_beats
    // Return 0 if there are not enough samples
size_t metronome::get_bpm() const {
    
        return m_beat_count;
}

void metronome::put_bpm(int bpmValue)
{
    m_beat_count = bpmValue;
}


