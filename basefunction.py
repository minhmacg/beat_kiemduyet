import pandas as pd
from rich.progress import Progress, BarColumn, MofNCompleteColumn, TimeElapsedColumn
def gettimecheck(x,tf='m'):
    if tf=="m" : return int(x[3:5])
    if tf=="h" : return int(x[:2])
    if tf=="s" : return int(x[6:])
def ghep(df):
    c=[]
    progress_columns = ("[progress.description]{task.description}",
                        BarColumn(),
                       MofNCompleteColumn(),
                        TimeElapsedColumn(),)
    
    with Progress(*progress_columns) as progress:
        task = progress.add_task("[red]Processing...", total=len(df))
        for i in range(len(df)-1):
            if pd.isna(df.loc[i,'content']):
               for y in range(len(df)):
                   a = abs(gettimecheck(df.loc[i,'time_hour'],'h')-gettimecheck(df.loc[y,'time_hour'],'h'))
                   b = abs(gettimecheck(df.loc[i,'time_hour'])-gettimecheck(df.loc[y,'time_hour']))
                   if a==0 and b<=1 and y!=i and df.loc[i,'timestamp_ms']== df.loc[y,'timestamp_ms'] and df.loc[i,'sender_name']==df.loc[y,'sender_name']:
                        df.loc[i,'content'] = df.loc[y,'content']
                        c.append(y)
            progress.update(task, advance=1,extra_info="Processing item {i}")
    df = df.drop(c)
    return df
def locpage(x):
    pages = {
        'beatvn' : ["Beatvn","Đoàn Việt", "Khánh Bùi", "Phuong Thao",
                    "Khánh Ly","Thang Le" , "Quỳnh Anh", "Quốc Huy",
                    "Bảo Trung", "Vũ Dũng", "Linh Phương", "Dương Lê",
                    "Vân Trang", "Vĩnh Hoàng","Tống Bùi Vĩnh Hoàng",
                    "Nguyễn Quỳnh Anh"],
        'beatnow' : ["Beat Now", "Minh Đức",'Nguyễn Viêt Anh', 'Nguyễn Việt Khánh', 'Nguyễn Đức Hậu', 'Nguyễn Đồng Tường', 'Minh Đức'],
        'cchn' : ["Chuyện của Hà Nội",'Khanh Huyen', 'Nguyễn Đức Trọng', 'Tuan Dinh Zin',
            'Thu Trangg', 'Phuong Thao', 'Khánh Ly', 'Hoàng Tuấn Anh', 'Vân Trang', 'My','Hồng Ngọc' ],
        'sgn' : ['Sài Gòn nghenn','Nguyễn Phương Nhi', 'Bánh Bòa', 'Đồng Lan Phương', 'Hi Ền', 'Khánh Bùi','Khánh Ly', 'My', 'Nguyễn Thành Vĩnh', 'Phuong Thao', 'Phương Nhi', 'Vân Trang','Quyen Tieu'],
        'itb' : ["Inside the Box",'Khánh Ly', 'Khánh Bùi', 'Giang Uyên','Tú Anh'],
        'kkn' : ["Kiến Không Ngủ", 'Huyy Anh', 'Bích Ngọc', 'Thu Trang', 'Khánh Ly',
                 'Khánh Bùi','Son Tung Nguyen','Vu Bich Ngoc','Bao-Linh Dong','Quỳnh Như'],
        'ct' : ['Cao Thủ','Khánh Ly Đinh','Quốc Quân Đinh','Linh Nguen','Nguyễn Văn Hòa', 'Hoàng Khánh Duy', 'Vân Trang','Trần Thị hồng Nhung'],
        'qc' : ['Quảng cáo','Phuong Thao', 'Khánh Ly', 'Khánh Bùi'],
        'showbeat' : ['Showbeat','Nguyễn Thành Vĩnh', 'Phương Nhi', 'Phương Nga Lê', 'Lệ Đỗ'],
        'tcshowbiz' : ['Tám chuyện Showbiz','Nguyễn Tuấn Thịnh','Thùy Linh']
        
        }

    for i,y in pages.items():
        if x in y: return y[0]


