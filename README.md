# group_14

readme_update_1 13.11.2023
automaatin toimintarakennesuunnitelmaa sekä er-kaavion lisäys
arvosanaan 1 tähtäävät toimenpiteet ajalle 13.11-19.11

• Debit kortti toteutettava. Debit kortilla ei ole luottoa eli tili ei saa mennä miinukselle. Jos saldo ei riitä, käyttäjä saa siitä ilmoituksen.  
• Pankkiautomaatille voi kirjautua antamalla kortin id-arvon ja PIN-koodin.  
• Käyttäjä voi katsoa saldonsa.  
• Käyttäjä voi nostaa, joko 20, 40, 50 tai 100 euroa.  
• REST APIin toteutettu kaikki automaatilla tarvittavat toiminnot  

Automaatin toimintarakenne ver 0.5 (karsittu arvosanaan 1 tähtääväksi)


### 1. syötä pin koodi

1.1 koodi oikein siirry kohtaan 2.  
1.2.1 koodi väärin -> teksti "Väärä pin, syötä uudelleen"  
1.2.2 koodi väärin uudestaan -> teksti "Väärä pin." ~~"seuraava väärä lukitsee kortin"~~  
1.2.3 koodi väärin 3. kerran -> teksti "Väärä pin, poista kortti" ~~"kortti lukittu, ole yhteydessä pankkiisi"~~  


### 2. valikko jossa näkyy 

2.1 katso saldo,  
~~2.2 katso tapahtumahistoriaa~~ // tulossa myöhemmin  
2.3 nosta tililtä,  
~~2.4 nosta luotolta.~~ // tulossa myöhemmin  
~~2.5 suorita maksu/tilisiirto~~ // tulossa myöhemmin  
~~2.6 talletus ominaisuus~~ // tulossa myöhemmin  

jos 2.1 -> näytä saldo ~~ja jos luotto niin näytä myös luoton tila~~ // tulossa myöhemmin  

~~jos 2.2 -> näytä 5 viimeisintä tapahtumaa ja painikkeella joko seuraavat tai edelliset 5 tapahtumaa~~ // tulossa myöhemmin  

jos 2.3 -> valitse näytöllä summa  

2.3.1 20€,  
2.3.2 40€,  
2.3.3 50€,  
~~2.3.4 80€,~~ // tulossa myöhemmin  
2.3.5 100€,  
~~2.3.6 150€,~~ // tulossa myöhemmin  
~~2.3.7 200€,~~ // tulossa myöhemmin  
~~2.3.8 syötä summa.~~ // tulossa myöhemmin  

~~jos 2.3.8 niin voi syöttää summan joka on jaollinen 20€ ja 50€ seteleille~~ // tulossa myöhemmin  

~~ja sama kaava luotolle 2.4 ->~~ // tulossa myöhemmin  

~~5 tähtäävä 2.5 tilinsiirto / maksupääte sekä 2.6 talletus ominaisuus~~ // tulossa myöhemmin  

### ER-kaavio:


<img src=ER-kaavio_kuva_ver1.2.png>

### Databaseen userin luominen ja kirjautuminen  

avaa ER-kaavio_ver1.2.mwb mysql:ssä ja luo tietokanta "create database bank_simul_r14" uniserverillä opettajan ohjeen mukaisesti.  
Tämän jälkeen diagrammissa "database" -> "synchronize model".

ja mysql komento luo käyttäjän:
<pre>
create user bankuser@'localhost' identified by 'bankpass';
grant all on bank_simul_r14.* to bankuser@'localhost';
</pre>

tämän jälkeen varmista että database.js pitää sisällään:  
<pre>
const mysql = require('mysql2');
const connection = mysql.createPool({
  host: 'localhost',
  user: 'bankuser',
  password: 'bankpass',
  database: 'bank_simul_r14'
});
module.exports = connection;
</pre>