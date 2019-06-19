<?php  

function exibeMapa($sql)
{
    global $db;
    $obras = $db->carregar($sql);
    ?>
    <script type="text/javascript" src="<?=GMAPS_API?>sensor=false"></script>
    <script type="text/javascript" src="/includes/maps/markerclusterer.js"></script>
    <script type="text/javascript">

        window.mapa.obras = <?=simec_json_encode($obras);?>

        window.mapa.markers = [];
        $(function(){
            var latlng = new google.maps.LatLng(-14.689881, -52.373047);
            var myOptions = {
                zoom: 4,
                center: latlng,
                mapTypeId: google.maps.MapTypeId.ROADMAP
            }
            window.mapa.map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);

            $.each(window.mapa.obras, function(i){
                latlng = new google.maps.LatLng($(this)[0].latitude,$(this)[0].longitude);
                marker = new google.maps.Marker({
                    position: latlng,
                    map: window.mapa.map,
                    title: $(this)[0].uf_muni
                });
                window.mapa.markers.push(marker);
            });

            var mc = new MarkerClusterer(window.mapa.map, window.mapa.markers);
        });

    </script>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td>
                <div id="map_canvas" style="width: 100%; height: 600px"></div>
            </td>
        </tr>
    </table>

    <?

}





function exibeXls($xlsparalizada, $sql){
    global $db;

    if($xlsparalizada){
        ob_clean();
        $cabecalho = array("ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão", "Percentual Supervisão", "Obra MI", "Data Homologação","Tipo de paralização", "Observações"," O município tem interesse em finalizar a obra? ","Dentro de que prazo o município irá concluir a obra? (N° de dias) "," O município já elaborou o novo processo licitatório para reinício e conclusão da obra, se sim, qual o prazo de encerramento, se não, qual o prazo de lançamento do Edital? (N° de dias) ", "O município irá concluir a obra por administração direta? "," O município precisa de orientação sobre como proceder para der continuidade aos serviços? ", "No cadastro da obra o endereço está correto?", "CEP (supervisão)","Logradouro (supervisão)","Número (supervisão)","Complemento (supervisão)","Município (supervisão)","UF (supervisão)", "Latitude (supervisão)", "Longitude (supervisão)", "Responsável", "CEP (original)","Logradouro (original)","Número (original)","Complemento (original)","Município (original)","UF (original)", "Latitude (original)", "Longitude (original)");
        $db->sql_to_xml_excel($sql,'obras_os',$cabecalho);
        die;

    }else{
        ob_clean();
        $cabecalho = array("ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão", "Percentual Supervisão", "Obra MI", "Data Homologação", "No cadastro da obra o endereço está correto?", "CEP (supervisão)","Logradouro (supervisão)","Número (supervisão)","Complemento (supervisão)","Município (supervisão)","UF (supervisão)", "Latitude (supervisão)", "Longitude (supervisão)", "Responsável", "CEP (original)","Logradouro (original)","Número (original)","Complemento (original)","Município (original)","UF (original)", "Latitude (original)", "Longitude (original)", "Possui problema grave?", "Descrição do Problema");

        $db->sql_to_xml_excel($sql,'obras_os',$cabecalho);
        die;
    }

}

function exibeLista($sql){
    global $db;
    $cabecalho = array("Ação","ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão","Percentual Supervisão", "Obra MI", "Data Homologação");


    //$db->monta_lista($sql,$cabecalho,100,5,'N','center','','','','', null);
    $db->monta_lista($sql, $cabecalho, 100, 5, 'N', 'center', 'N', "formulario", '', '', $cache, array('table-responsive' => true));

}