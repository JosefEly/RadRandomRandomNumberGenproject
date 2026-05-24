#x
import discord
import random
import serial
import datetime
import math

from discord.ext import tasks
import asyncio
import traceback

arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=10)


bits = []
bit_lock = asyncio.Lock()
ideal_length = 6000
midnight = datetime.time(5,0,0)
help_text = """
Generates random numbers from a home-built alpha-particle detector.
Prompt me through $roll (number of dice, integer)d(type of dice, integer).
Returns in the format:

total
[roll1, roll2, etc]

For example:
$roll 1d6
could return
4
[3, 1]

Note that if any of the rollls equal zero, the detector is out of stored bits, and you will need to wait 5 minutes or so for the bound americium fairy to supply new enthropy
"""


def roll_dice(dice_to_roll):
        arduino.reset_input_buffer()
        message = dice_to_roll + '\n'
        arduino.write(bytes(message, 'utf-8'))
        data = str(arduino.read_until(), 'utf-8')
        print(data)
        rolls = data.split(",")
        rolls_clean = []
        for roll in rolls:
                roll = roll.strip("\rn")
                rolls_clean.append(roll)
        rolls_int = [int(roll) for roll in rolls_clean]
        return rolls_int

def listconvert(input_string):
        new_list = input_string.split('d')
        int_list = []
        for i in new_list:
                int_list.append(int(i))
        return int_list
def roll_dN(dice_size):
        roll_total = 0
        dice_max = math.ceil(math.log(dice_size, 2))
        #print("dicemax " + str(dice_max))
        if dice_max > len(bits): return 0
        #print("sliced " + str(bits[:dice_max]))
        for i in range(dice_max):
                roll_total += (pow(2,i)*bits.pop(0))
        roll_total += 1
        #print("total " + str(roll_total))
        if roll_total <= dice_size:
                return roll_total
        else: return roll_dN(dice_size)

def roll_dice_stored(dice_to_roll):
        roll_param = listconvert(dice_to_roll)
        #print("param")
        print(roll_param)
        dice_rolls = []
        for i in range(roll_param[0]):
                dice_rolls.append(roll_dN(roll_param[1]))
        return dice_rolls


        """
        arduino.reset_input_buffer()
        message = dice_to_roll + '\n'
        arduino.write(bytes(message, 'utf-8'))
        data = str(arduino.read_until(), 'utf-8')
        print(data)
        rolls = data.split(",")
        rolls_clean = []
        for roll in rolls:
                roll = roll.strip("\rn")
                rolls_clean.append(roll)
        rolls_int = [int(roll) for roll in rolls_clean]"""

"b'1d4\r\n'"

async def replenish_bits():

        time = datetime.datetime.now()
        #print("going")
        while len(bits) < ideal_length:
                arduino.reset_input_buffer()
                message = '150d2' + '\n'
                #print('sent')
                arduino.write(bytes(message, 'utf-8'))
                data = str(arduino.read_until(), 'utf-8')
                bitlist = data.split(",")
                int_bits = []
                for number in bitlist:
                        number = number.strip("\rn")
                        int_bits.append(int(number)-1)
                for num in int_bits:
                        bits.append(num)
                print(len(bits))
        #print(bits)
        print(datetime.datetime.now()-time)
class MyClient(discord.Client):
        async def on_ready(self):
                print('Logged on as', self.user)
                if not check_bits.is_running(): check_bits.start()
                else: print("check_bits task was already running")
                if not check_bits_midnight.is_running():
                    check_bits_midnight.start()
                    print("Started 12 AM bit top-off task")
                else:
                        print("Midnight task already running")

        def clean_content(self, data):
                data = data.replace(" ", "")
                data = data.replace("$roll", "")
                ##cleaned_data = data.split("d")
                print(data)
                return data

        async def on_message(self, message):
                async with bit_lock:
                        # don't respond to ourselves
                        content = message.content
                        content = content.lower()
                        if message.author == self.user:
                                return

                        if "$roll" in content:
                                content = content.replace(" ", "")
                                content = content.replace("$roll", "")
                                items = self.clean_content(content)
                                print(len(bits))
                                final_rolls = roll_dice_stored(items)
                                await message.channel.send(sum(final_rolls))
                                await message.channel.send(final_rolls)
                                print(len(bits))
                        if "$help" in content:
                                await message.channel.send(help_text)
                                #check_bits.start()
@tasks.loop(seconds=30)
async def check_bits():
        async with bit_lock:
                #print("starting1")
                if len(bits) <= 0.01*ideal_length:
                        print("starting replenishment")
                        await replenish_bits()
'''
        except Exception as e:
                print(f"Error in check_bits: {e}")
                traceback.print_exec()

'''
@tasks.loop(time=midnight)
async def check_bits_midnight():
        print("MIDRUN")
        async with bit_lock:
                if len(bits) < ideal_length:
                        await replenish_bits()

intents = discord.Intents.default()
intents.message_content = True
client = MyClient(intents=intents)
client.run("API KEY GOES HERE")
"""
import serial
import time
arduino = serial.Serial(port='/dev/cu.usbserial-210', baudrate=115200, timeout=.1)
time.sleep(3)
def write_read(x):
        arduino.write(bytes(x, 'utf-8'))
        time.sleep(0.05)
        data = arduino.readline()
        return data
#num = input("Enter a number: ") # Taking input from user
value = write_read("Test")
print(value) # printing the value
"""
