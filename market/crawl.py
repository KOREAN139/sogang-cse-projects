import requests
import json

# get request via requests
# req = requests.get('https://api.coinmarketcap.com/v1/ticker?limit=1')

coins = ['bitcoin', 'ethereum', 'bitcoin-cash', 'ripple', 'litecoin', 'dash', 'monero', 'bitcoin-gold', 'eos', 'qtum', 'ethereum-classic', 'zcash'];
def get_info(name):
    status = []
    if name == '':
        for coin in coins:
            req = requests.get('https://api.coinmarketcap.com/v1/ticker/'+coin+'/?convert=KRW')
            # get json
            t = req.json()
            #print(t)
            status.append(t)
    else:
        req = requests.get('https://api.coinmarketcap.com/v1/ticker/' + name + '/?convert=KRW')
        t = req.json()
        status.append(t)
    return status