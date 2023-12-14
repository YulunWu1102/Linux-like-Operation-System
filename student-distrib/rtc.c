#include "rtc.h"
#include "lib.h"
#include "i8259.h"

static volatile uint32_t rtc_int_counter;
static uint32_t vir_rtc_interrupt[3];
static uint32_t vir_rtc_counter[3];

#define VIRTULIZE_RTC  
/* void rtc_initial()
 * Description: set up the rtc for inturrupt
 * Inputs:   NA   
 * Return Value: NA
 * Function: Initialize the rtc */
void rtc_initial() {

    #ifdef VIRTULIZE_RTC
    cli();
    // write B first
        int i; 
        //disable interrupts first
        
        // select reg b and disable NMI
        outb(REG_B,RTC_PORT);
        //read the cur value of reg b
        unsigned char prev = inb(CMOS_PORT);
        //set the index again 
        outb(REG_B,RTC_PORT);
        // write the previous value ORed with 0x40
        outb(prev | ORed_OR, CMOS_PORT);  
    //Turning on IRQ8
        enable_irq(RTC_IRQ);

        for(i = 0; i < 3; i++) {
            vir_rtc_interrupt[i] = 0;
            vir_rtc_counter[i] = 0;
        }

        
    //set up the default frequency
        set_up_frequency(MAX_FRE);
        sti();
        // rtc_open("rtc");
        return;
    #else 
    // write B first
        //disable interrupts first
        rtc_int_counter = 0;
        cli();
        // select reg b and disable NMI
        outb(REG_B,RTC_PORT);
        //read the cur value of reg b
        unsigned char prev = inb(CMOS_PORT);
        //set the index again 
        outb(REG_B,RTC_PORT);
        // write the previous value ORed with 0x40
        outb(prev | ORed_OR, CMOS_PORT);

        sti();
    //Turning on IRQ8
        enable_irq(RTC_IRQ);
    //set up the default frequency
        set_up_frequency(MAX_FRE);
        return;
    #endif 
}

/* void rtc_handler()
 * Description: set up the rtc handler for inturrupt
 * Inputs:   NA   
 * Return Value: NA
 * Function: process the rtc handler and send the EOI to PIC*/
void rtc_handler() {

    #ifdef VIRTULIZE_RTC
        cli();
        int i;
        send_eoi(RTC_IRQ);
        if(vir_rtc_counter[curr_rid] >= currTerminal[curr_rid].counter) {
            // an virtual interrupt occur
            vir_rtc_interrupt[curr_rid] = 1;
            // reset the counter 
            vir_rtc_counter[curr_rid] = 0 ;
        } else {
            for(i = 0; i < 3; i++) {
                vir_rtc_counter[i]++;
            }
        }
        outb(REG_C,RTC_PORT);
        inb(CMOS_PORT);
        sti();
    #else 

        // send interrupt ack to PIC
        rtc_int_counter++;
        outb(REG_C,RTC_PORT);
        inb(CMOS_PORT);
        // test_interrupts();
        // printf("1");
        send_eoi(RTC_IRQ);
    #endif
}


/* rtc_open
 * Description:  provides access to the file system rtc through system call 
 * Inputs:   filename   
 * Return Value:0 if success
 * Function: process the rtc handler and send the EOI to PIC*/
int32_t rtc_open(const int8_t *filename){
    #ifdef VIRTULIZE_RTC
        set_up_frequency(MAX_FRE);

        vir_rtc_counter[curr_rid] = 0;
        currTerminal[curr_rid].counter = MAX_FRE/MIN_FRE - 1;
        return 0;
    #else
        set_up_frequency(MIN_FRE);
        return 0;
    #endif
}

/* rtc_clsoe
 * Description:  closes the specified file descriptor and makes it available for return from later calls to open.
 * Inputs:   fd  
 * Return Value: 0 if success
 * Function: process the rtc handler and send the EOI to PIC*/
int32_t rtc_close (int32_t fd){
    return 0;
}

/* rtc_read
 * Description:  provides access to the file system rtc through system call 
 * Inputs:   fd, buf, nbytes   
 * Return Value:0 if success
 * Function: process the rtc handler and send the EOI to PIC*/
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){

    #ifdef VIRTULIZE_RTC
        while(!vir_rtc_interrupt[curr_rid]) {}
        vir_rtc_interrupt[curr_rid] = 0;
        return 0; 
    #else
        rtc_int_counter = 0;
        // wait for the interrupt to come
        while (!rtc_int_counter) {}
        rtc_int_counter = 0;
        return 0;
    #endif
}

/* rtc_write
 * Description:  provides access to the file system rtc through system call 
 * Inputs:   fd, buf, nbytes   
 * Return Value:0 if success
 * Function: process the rtc handler and send the EOI to PIC*/
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){
    #ifdef VIRTULIZE_RTC
        cli();
        if (nbytes != sizeof(uint32_t) || buf == NULL) return -1;
        int freq = *((int *) buf);
        // set_up_frequency(freq);
        currTerminal[curr_rid].counter = MAX_FRE / freq - 1;
        sti();
        return 0;
    #else
        // check failing conditions
        if (nbytes != sizeof(uint32_t) || buf == NULL) return -1;
        int freq = *((int *) buf);
        set_up_frequency(freq);
        return 0;
    #endif
}

/* set_up_frequency
 * Description: set up the rtc frequency for inturrupt
 * Inputs:   NA   
 * Return Value: NA
 * Function: process the rtc handler and send the EOI to PIC*/
void set_up_frequency(int32_t frequency) {
    //check if freq is in the range
    if (frequency < MIN_FRE || frequency > MAX_FRE) return;
    // Check if frequency is a power of 2
    if (frequency & (frequency - 1)) return;

    unsigned char rate = MAX_RATE + 1 - log2(frequency) ;
    // set rate not obave 15 
    rate &= RATE_MASK;
    // determine whether it is faster than 3
    if (rate < 3)  return;

    // start critical section
    cli();
    // select reg a and disable NMI
    outb(REG_A, RTC_PORT);
    //read the cur value of reg a
    unsigned char prev = inb(CMOS_PORT);
    //set the index again 
    outb(REG_A,RTC_PORT);
    // write only our rate to A
    outb((prev & 0xF0) | rate, CMOS_PORT);
    // end the critical point
    sti();
}
