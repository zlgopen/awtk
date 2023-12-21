GITHUB=github.com
GITEE=gitee.com

for f in $(cat repos.txt)
do
  echo $f
  if [ -e $f ]
  then
    cd $f
    git pull
    cd -
  else
    git clone https://$GITEE/zlgopen/$f
  fi
done


