
const currentUrl = new URL(window.location.href);
const ip = currentUrl.searchParams.get('ip');
console.log(window.location.href);
console.log(ip);
document.getElementById('togglePassword').addEventListener('click', function (e) {
  const password = document.getElementById('pwd');
  const type = password.getAttribute('type') === 'password' ? 'text' : 'password';
  const eye=document.getElementById("ojo");
  eye.classList.toggle("fa-eye-slash")
  password.setAttribute('type', type);
  
});

function gettemp(){
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "http://"+ip+"/temperature", true);
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Temperatura").innerHTML = this.responseText;
      document.getElementById('tempbar').style.width=this.response +'%'
      document.getElementById('tempbar').style.backgroundColor='rgb('+Math.floor((255/20)*parseFloat(this.response))+',200,100)'
    }
  };

  xhttp.send();
}
function gethum(){
    var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Humidity").innerHTML = xhttp.responseText;
      document.getElementById('hbar').style.width=this.response+'%';
      document.getElementById('hbar').style.backgroundColor='rgb(100,200,'+Math.floor(((205/100)*parseFloat(this.response))+50)+')';
    }
  };
  xhttp.open("GET", "http://"+ip+"/humidity", true);
  xhttp.send();
}

document.getElementById('insesion').addEventListener('click',async function(event){
    event.preventDefault();
    const uname=document.getElementById('username').value;
    const pwd=document.getElementById('pwd').value;
    const dat=new FormData;
    dat.append('nombre_usuario', uname);
    dat.append('contraseña', pwd);

    var xhttp = new XMLHttpRequest();
    xhttp.open("post","http://"+ip+"/inicio", true);
    xhttp.onload=function(){
      if (xhttp.response === "yes"){
        document.getElementById('in').style.display='none';
        document.getElementById('act').style.display='block';
      }
    }
    xhttp.send(dat);

});

setInterval(gettemp, 2000);
setInterval(gethum, 2000);
function fan(){
  var xhttp = new XMLHttpRequest();
  xhttp.open("post", 'http://'+ip+'/int');
  xhttp.onload=function(){
    if(xhttp.response==='on'){
      document.getElementById('bov').classList.remove('inactive');
      document.getElementById('bov').classList.add('active');
    }
    else if(xhttp.response==='off'){
      document.getElementById('bov').classList.remove('active');
      document.getElementById('bov').classList.add('inactive');
    }
  }
  xhttp.send();
}
function vent(){
  var xhttp = new XMLHttpRequest();
  xhttp.open('post', 'http://'+ip+'/ven', true);
  xhttp.onload=function(){
    if (xhttp.response==='1'){
      document.getElementById('bove').classList.remove('inactive');
      document.getElementById('bove').classList.add('active');
    }
    else if(xhttp.response==='0'){
      document.getElementById('bove').classList.remove('active');
      document.getElementById('bove').classList.add('inactive');
    }
  }
  xhttp.send();
}
function st(){
  var xhttp=new XMLHttpRequest();
  xhttp.open('post','http://'+ip+'/mtemp');
  const dat=new FormData;
  dat.append("ntemp", document.getElementById('maxtemp').value);
  xhttp.send(dat);
}