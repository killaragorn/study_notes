import urllib.request
import json

# url = 'http://www.fishc.com'
# response  =  urllib.request.urlopen(url)
# print(response.read().decode())

# url = 'http://www.placekitten.com/g/300/300'
# response = urllib.request.urlopen(url)
# with open('cat.png','wb') as file:
#     file.write(response.read())

url  = 'http://fanyi.so.com/index/search?eng=1&validate=&ignore_trans=0&query=test'
# from=zh&to=en&query=%E6%88%91%E7%88%B1%E4%BD%A0&transtype=translang&simple_means_flag=3
data = {
    'from': 'zh',
    'to': 'en',
    'query': 'i love you',
    'transtype' : 'translang',
    'simple_means_flag' : 3
}

headers = {
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36 Edg/87.0.664.6',
    'cookies' :'QiHooGUID=5CF1569F1E15E45431F868DA979ACB87.1609747873100; Q_UDID=38684bba-4d86-8042-e1db-7729d2329030; __guid=144965027.2099596189748525300.1609747874210.8577; __huid=11rrPk8t4AWmh%2B51%2FRY%2BHUtGHlyzvIlsWX4M%2FU%2FoEmD1k%3D; gtHuid=1; count=2',
    'pro' : 'fanyi'
}


request = urllib.request.Request(url=url,headers=headers)
responese = urllib.request.urlopen(request,data=b'eng=1&validate=&ignore_trans=0&query=hold')
ret = json.loads(responese.read().decode())
print(ret['data']['explain'])