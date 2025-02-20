#!/usr/bin/python3
import os
import sys
import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

TEST_TIMEOUT_S=58
SCORE_PER_TEST=2

if len(sys.argv) not in [3,4]:
    print('Usage: ./webdriver_test.py <proxy_port> <tiny_port>')
    sys.exit(1)

proxy_port=sys.argv[1]
tiny_port=sys.argv[2]
is_driver=(len(sys.argv)==4 and sys.argv[3]=='--driver')
is_headless=('DISPLAY' not in os.environ)
should_collect_score=(is_driver or is_headless)

print('= launching chrome',time.time())

opt=webdriver.ChromeOptions()
if is_headless:
    print('no display available. going headless.')
    opt.add_argument('--headless')
opt.add_argument('--no-sandbox')
opt.add_argument('--disable-gpu')
opt.add_argument('--disable-dev-shm-usage')
opt.add_argument('--proxy-server=127.0.0.1:%s'%proxy_port)
opt.add_argument('--proxy-bypass-list=*.google.com') # chrome will auto-connect accounts.google.com
opt.add_argument('--user-agent=proxylab_test_'+('runner' if should_collect_score else 'manual'))
driver=webdriver.Chrome(options=opt)

print('= loading page',time.time())
driver.set_page_load_timeout(100)

# we use nip.io trick because chrome will bypass proxy for localhost :(
driver.execute_script('location.href="http://127.0.0.1.nip.io:%s/browser-testbench/index.html"'%tiny_port)
try:
    element=WebDriverWait(driver, 7).until(
        EC.presence_of_element_located((By.CSS_SELECTOR, "#grader-run-btn"))
    )
finally: # print debug info even if failed
    print('url:',driver.current_url)
    print('title:',driver.title)    

time.sleep(.5)

print('= running test',time.time())

element.click()

if not should_collect_score:
    print('= check your score on the webpage!')
    sys.exit(0)

element=WebDriverWait(driver, TEST_TIMEOUT_S).until(
    EC.presence_of_element_located((By.CSS_SELECTOR, "#grader-passed-num--DO-NOT-FAKE-THIS-OR-YOU-WILL-GET-SCORE-DEDUCTION"))
)

print('= retrieving score',time.time())

score=element.text.strip()
print('passed tests reported by browser:',score)
if is_driver:
    with open('webdriver_score--DO-NOT-FAKE-THIS-OR-YOU-WILL-GET-SCORE-DEDUCTION.txt','w') as f:
        if score.isdigit():
            written_score=int(score)*SCORE_PER_TEST
            f.write('%d\n'%written_score)
            print('so you get %d score'%written_score)
        else:
            print('score invalid! ('+score+')')

element=WebDriverWait(driver, 5).until(
    EC.presence_of_element_located((By.CSS_SELECTOR, "#grader-log-container"))
)
try:
    print('Log:\n',element.text)
except: # some terminal can only output ascii
    print('Log (ascii):\n',element.text.encode('ascii',errors='replace').decode('ascii'))

print('= finished',time.time())

driver.quit()

