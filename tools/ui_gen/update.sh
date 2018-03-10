./xml_to_ui window1.xml window1.data
./xml_to_ui dialog1.xml dialog1.data

for f in *.xml;
do
echo ./xml_to_ui $f ${f/xml/data}
./xml_to_ui $f ${f/xml/data}
done

mv -v *.data ../../demos/res/ui/

