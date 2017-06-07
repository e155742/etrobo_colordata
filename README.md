カラーセンサーからの情報を画面に出力する
====
Redmine [チケット#3631](https://redmine.ie.u-ryukyu.ac.jp/issues/3631)  

0.5秒おきに更新されます。  
RGB値のMAXが255,255,255ではないので、正規化してあります。  
[Redmine wiki/Spec](https://redmine.ie.u-ryukyu.ac.jp/projects/etrobo2017-teamtwd/wiki/Spec)を参照。  
EV3内の**su_color**がこれです
### 表示項目
- 反射光
- RGB
    - 赤
    - 緑
    - 青
- HSV
    - 色相
    - 彩度
    - 明度
- 色判別
