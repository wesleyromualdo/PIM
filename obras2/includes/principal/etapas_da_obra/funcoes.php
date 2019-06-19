<?php /* ****arquivo vazio**** */

function validaDataPHP($data) {
    $data = new DateTime($data);
}


function obraAditivada($obrid)
{
    $obra = new Obras($obrid);
    $crtid = $obra->pegaContratoPorObra($obrid);

    $ctr = new Contrato($crtid);
    $dadosContrato = $ctr->getDados();

    if($dadosContrato['ttaid'])
        return true;

    return false;
}




function gerarXlsItens() {
    global $db;

    $obrid   = $_SESSION['obras2']['obrid'];
    $itens   = new ItensComposicaoObras();
    $arItens = $itens->getItemComposicaoByObra($obrid);
    $arItens = !empty($arItens) ? $arItens : array();

    $arrCronograma = array();
    $vl_total_crono = 0;
    foreach ($arItens as $key => $value) {
        $vl_total_crono += $value['icovlritem'];
    }

    foreach ($arItens as $key => $value) {
        $data_i = date_create($value['icodtinicioitem']);
        $data_i = date_format ( $data_i, 'd/m/Y' );
        $data_f = date_create($value['icodterminoitem']);
        $data_f = date_format ( $data_f, 'd/m/Y' );
        $vl     = number_format($value['icovlritem'], 2, ',', '.');
        if($vl_total_crono != 0){
            $percent = $value['icovlritem']*100/$vl_total_crono;
            $percent = number_format($percent, 2, ',', '.');
        }else{
            $percent = '0,00';
        }
        $vl2     = number_format($vl_total_crono, 2, ',', '.');
        $arrCronograma[] = array($value['itcdesc'], $data_i, $data_f, $vl, $percent, $vl2);
    }


    $cabecalho = array( "DescriÃ§Ã£o do Item", "Data de InÃ­cio", "Data de TÃ©rmino", "Valor do Item", "(%) Referente a Estrutura", "Total Obra");

    ob_clean();
    ini_set("memory_limit", "512M");
    header("Content-type: application/excel; name=CronogramaObra.xls");
    header("Content-Disposition: attachment; filename=CronogramaObra.xls");
    $db->sql_to_xml_excel($arrCronograma, 'CronogramaObra', $cabecalho);
}




/**
 * Verifica se o cronograma possui alguma trava de ediÃ§Ã£o.
 * O cronograma da obra convencional deve ser travado para modificaÃ§Ãµes pelo supervisor unidade apÃ³s a primeira vistoria.
 *
 * @param int $obrid
 * @return boolean - TRUE nÃ£o pode editar, FALSE pode
 */
function possuiTravaCronograma($obrid){
    /**
    -> Verificar se possui vistorias;
     */

    $obra     = new Obras();
    $obra->carregarPorIdCache($obrid);
    $vistoria = new Supervisao();

    $ultima_vistoria               = $vistoria->pegaUltSupidByObra($obrid);

    if($ultima_vistoria){
        return true;
    }

    return false;

}



