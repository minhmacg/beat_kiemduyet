import basefunction
import convertjson
import json
import openpyxl
import re
import pandas as pd
from pandas import json_normalize
from rich import print as rprint
### lay ten page tu input
inpage=" "
pages=[]
print('nhap ten page, enter 2 lan khi xong')
while inpage!="" :
    inpage=input('nn: ')
    pages.append(inpage)
pages.pop(-1)
print(pages)
text=input('ten file text: ')
text=text+".txt" if ".txt" not in text else text

# Convert json
print('Converting Json form hex to unicode')
convertjson.main()

# Load the JSON data from the file
with open("output.json", "r") as file:
    data = json.load(file)

# Extract the 'messages' key from the JSON data
messages = data["messages"]
print    
#######

a=[]
b=[]
img=[]
vid=[]
share=[]
for i in messages:
    if 'reactions' in i.keys():
        react=[]
        actor=[]
        for y in i['reactions']:
            rprint('[green]Loaded reaction[/green] ',y['reaction'],' --- actor ',y['actor'])
            react.append(y['reaction'])
            actor.append(y['actor'])
    else:
        react=[]
        actor=[] 
    a.append(react)
    b.append(actor)

for i in messages:
    if 'photos' in i.keys():
        url2=[]
        for y in i['photos']:
            rprint('[green]Loaded photos:[/green] ',y['uri'][0:100])
            url2.append(y['uri'])
        img.append(url2)
    else: img.append('')
    if 'videos' in i.keys():
        url2=[]
        for y in i['videos']:
            print('[green]Loaded vids:[/green] ',y['uri'][:100])
            url2.append(y['uri'])
        vid.append(url2)
    else: vid.append('')

for i in messages:
    if 'share' in i.keys() and i['share']['link'].find('facebook.com')>0:
       share.append(i['share']['link'])
    else: share.append('')
u={'reacts':a,
    'actors':b,
    'imgs':img,
    'vids':vid,
    'share': share}
ff=pd.DataFrame(data=u,columns=['reacts','actors','imgs','vids','share']) 
ff.to_excel('test/test1.xlsx')
####

df = json_normalize(messages,meta=['timestamp_ms','time_hour','sender_name','content'])
df= pd.concat([df,ff],axis=1)
df = df[['timestamp_ms','time_hour','sender_name','content','imgs','vids','reacts','actors','share']]

####
df = df[df['reacts'].apply(lambda x: len(x))!=0]
df.reset_index(drop=True,inplace=True)
print('Filtered reaction-only messages')
### loc link
   
df['link']=df['content'].apply(lambda x: x if (pd.notna(x) and x.startswith('http')) else '')
for i in range(len(df)): 
    if df.loc[i,'link']!='': df.loc[i,'content']=' ' 
df.loc[:,'content',] = df.loc[:,'content'].apply(lambda x: re.sub('\n',' ',x) if pd.notna(x) else x)

#### 
##### loc url anh
rprint('[red]Getting file name from url[/red]')
def locten(x):
    for i in range(len(x)):
        x[i]=re.search(r"([^/]+)$", x[i]).group(1)
        x[i]=x[i].split('_n_')[0]
    return x    
df['imgs']=df['imgs'].apply(lambda x: locten(x) if len(x)>0 else x)
df['vids']=df['vids'].apply(lambda x: locten(x) if len(x)>0 else x)
### test loc file 2
#### check kdv
def checkkdv(x):
    if 'Đức Bách' in x and 'Thành Đạt' in x : return ''
    elif ('Đức Bách' in x or 'Thành Đạt' in x):
        if len(x)==1: return ','.join(x)
        else:
            if 'Đức Bách' in x : return 'Đức Bách'
            else: return 'Thành Đạt'  
    else: return ''

df['actors'] = df.loc[:,'actors'].apply(checkkdv).astype(str)
df['actors'] = df['actors'].apply(lambda x: x.split()[-1] if len(x)>0 else x)


df.to_excel("test/test2.xlsx")
####
print('Ghep tin nhan .... ')
df = basefunction.ghep(df)
##### lay ten page
print('Filtering page name...')
df['page']=df['sender_name'].apply(lambda x: basefunction.locpage(x))
df = df[pd.notna(df['page'])]
#### lay link url

with open(text,'r') as f:
    data=f.readlines()
linkurl = pd.Series(data)
stra= '\n'
def getlink(x):
    link=[]
    for i in x:
        for j in linkurl:
            if i in j : link.append(j.replace(',\n',''))
    return stra.join(link)
print('Getting file url...')
df['imgs'] = df['imgs'].apply(lambda x: getlink(x) if type(x)!=float else x)
df['vids'] = df['vids'].apply(lambda x: getlink(x) if type(x)!=float else x)
df.loc[:,'reacts'] = "Duyệt" 
df = df.loc[:,['timestamp_ms','page','sender_name','content','imgs','vids','link','actors','reacts']]
print(df)
#### xuat ra excel
print('Xuat ra Excel.....')
filename = pages[0] + '.xlsx'
df.to_excel(filename)
rprint('[green]Done exporting to[/green] ',filename)

