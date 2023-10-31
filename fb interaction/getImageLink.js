var imgs= []
var error = 0 
const config = {
    btnClass: "x6s0dn4 x78zum5 x197sbye xyamay9 x1pi30zi x1l90r2v x1swvt13 x1n2onr6 x1k90msu x6o7n8i x9lcvmn x6my1t9 xiwuv7k",
    mainImgClass:'x9f619 x78zum5 x1iyjqo2 x5yr21d xl56j7k x6ikm8r x10wlt62 x1n2onr6 xh8yej3 xvj1nn1 x1nd34w2',
    time: 100
}
var imgLinksFunction = []

function stop(){
    console.log('imgLinksFunction', imgLinksFunction)
    imgLinksFunction.map(func=>{
        clearInterval(func)
        imgLinksFunction = []
    })
}

function getLinks(pic){
    imgLinksFunction.push(setInterval(()=>{ 
        var btn = document.getElementsByClassName(config.btnClass); 
        if(btn[0]){
            var mainImg = document.getElementsByClassName(config.mainImgClass)[0].getElementsByTagName('img')
            var mainVideo = document.getElementsByTagName('video')
            if(mainImg[0] || mainVideo[0]){
                var src = ''
                if(mainImg[0]){
                    src = mainImg[0].src
                }
                if(mainVideo[0]){
                    src = mainVideo[0].src
                }
                imgs.push(src)
                console.log("success", src)
                var btnLeft = btn[0];
                btnLeft.click()
            }else{
                error = error + 1 
                console.error("Lấy lại ảnh đằng sau link ảnh", imgs[imgs.length-1])
                stop()
                setTimeout(()=>getLinks(), 1000)
            }
        }
        if(imgs[imgs.length-1]==imgs[imgs.length-2] || imgs[imgs.length-1].includes(pic))  {
            stop()
            console.log(imgs)
            console.warn("Tổng ảnh lấy lại", error)
            alert('Success '+imgs.length +" files loaded!")
        }
    },
    config.time )
    ) 
}
getLinks("394326956_6205957172842388_1304812809741396730")
