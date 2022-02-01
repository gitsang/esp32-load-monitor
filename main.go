package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	"github.com/shirou/gopsutil/cpu"
	"github.com/shirou/gopsutil/mem"
)

const (
	esp32host = "192.168.5.39"
)

func main() {
	go func() {
		for {
			cpuloads, _ := cpu.Percent(time.Duration(time.Second), false)
			cpuload := cpuloads[0]
			cpu8bit := cpuload / 100 * 256
			resp, _ := http.Get(fmt.Sprintf("http://%s:%d/left/%f", esp32host, 80, cpu8bit))
			body, _ := ioutil.ReadAll(resp.Body)
			log.Println("cpuload: ", cpuload, ", cpu8bit: ", cpu8bit, ", resp: ", string(body))
		}
	}()

	go func() {
		for {
			memInfo, _ := mem.VirtualMemory()
			memload := memInfo.UsedPercent
			mem8bit := memload / 100 * 256
			resp, _ := http.Get(fmt.Sprintf("http://%s:%d/right/%f", esp32host, 80, mem8bit))
			body, _ := ioutil.ReadAll(resp.Body)
			log.Println("memload: ", memload, ", mem8bit: ", mem8bit, ", resp: ", string(body))
			time.Sleep(time.Second)
		}
	}()

	select {}
}
