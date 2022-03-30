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
	esp32host = "loadoutput.dns.army"
)

func main() {
	go func() {
		for {
			cpuloads, _ := cpu.Percent(time.Duration(time.Second), false)
			cpuload := cpuloads[0]
			cpu8bit := cpuload / 100 * 256
			resp, err := http.Get(fmt.Sprintf("http://%s:%d/left/%f", esp32host, 80, cpu8bit))
			if err != nil {
				log.Println("[error] send cpu load failed")
				continue
			}
			body, err := ioutil.ReadAll(resp.Body)
			if err != nil {
				log.Println("[error] http read body failed")
				continue
			}
			log.Println("[info] cpuload: ", cpuload, ", cpu8bit: ", cpu8bit, ", resp: ", string(body))
		}
	}()

	go func() {
		for {
			time.Sleep(time.Second)

			memInfo, _ := mem.VirtualMemory()
			memload := memInfo.UsedPercent
			mem8bit := memload / 100 * 256
			resp, err := http.Get(fmt.Sprintf("http://%s:%d/right/%f", esp32host, 80, mem8bit))
			if err != nil {
				log.Println("[error] send mem load failed")
				continue
			}
			body, err := ioutil.ReadAll(resp.Body)
			if err != nil {
				log.Println("[error] http read body failed")
				continue
			}
			log.Println("[info] memload: ", memload, ", mem8bit: ", mem8bit, ", resp: ", string(body))
		}
	}()

	select {}
}
