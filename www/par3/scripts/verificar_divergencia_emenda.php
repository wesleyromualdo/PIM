<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/simec_funcoes.inc";
require_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . 'www/par3/_constantes.php';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

$filtro = "";
if( $_REQUEST['emenda'] ){
    $filtro = " and emecod = '{$_REQUEST['emenda']}' ";
}

if( empty($_REQUEST['original']) && empty($_REQUEST['backup']) ) $_REQUEST['original'] = 'S';

$sql = "SELECT emeid, emecod, emeano FROM emenda.emenda e WHERE e.emeano = '2018' $filtro order by emeid";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

$htmlDetalhe = '';
$htmlDetalhe_bk = '';
foreach ($arrDados as $key => $v) {
    
    /*$sql = "SELECT count(*)
            FROM emenda.emendadetalhe ed
            INNER JOIN(
            	SELECT emdid, emeid, emdvalor, gndcod, mapcod, foncod FROM emenda.emendadetalhe_bk_01 where emeid = {$v['emeid']} ORDER BY emdid
            ) bk ON bk.emeid = ed.emeid
            INNER JOIN emenda.emenda e ON e.emeid = ed.emeid
            WHERE (ed.gndcod <> bk.gndcod OR ed.emdvalor <> bk.emdvalor)
                and ed.emeid = {$v['emeid']}
            	AND e.emeano = '{$v['emeano']}'
            	AND e.etoid = 1";
    $boDiferenca = $db->pegaUm($sql);*/
    
    //if($boDiferenca > 0){
    $key % 2 ? $cor = "#FFFFGG" : $cor = "";
    
    $sql = "SELECT emdid, emeid, gndcod, foncod, mapcod, emdvalor, emddataalteracao, usucpfalteracao, emdtipo, emdimpositiva, emdtipodescentralizacao, emdliberacaosri, emddtultimoenviosiop, emdstatus, emddotacaoinicial
            FROM emenda.emendadetalhe where emeid = {$v['emeid']} order by emdid";
    $arrEmendaDetalhe = $db->carregar($sql);
    $arrEmendaDetalhe = $arrEmendaDetalhe ? $arrEmendaDetalhe : array();
    
    $sql = "SELECT emdid, emeid, gndcod, foncod, mapcod, emdvalor, emddataalteracao, usucpfalteracao, emdtipo, emdimpositiva, emdtipodescentralizacao, emdliberacaosri, emddtultimoenviosiop, emdstatus, emddotacaoinicial
            FROM emenda.emendadetalhe_bk_01 where emeid = {$v['emeid']} order by emdid";
    $arrEmendaDetalhe_bk = $db->carregar($sql);
    $arrEmendaDetalhe_bk = $arrEmendaDetalhe_bk ? $arrEmendaDetalhe_bk : array();
    
    if( $_REQUEST['original'] == 'S' ){
    //if( sizeof($arrEmendaDetalhe) <> sizeof($arrEmendaDetalhe_bk)){
        foreach ($arrEmendaDetalhe as $detalhe) {
            $sql = "SELECT ede.edeid, eb.enbcnpj, eb.enbnome, ede.edevalor, ede.edestatus FROM emenda.emendadetalheentidade ede
                        	INNER JOIN emenda.entidadebeneficiada eb ON eb.enbid = ede.enbid
                        WHERE ede.emdid = {$detalhe['emdid']} order by eb.enbcnpj";
            $arrEntidade = $db->carregar($sql);
            $arrEntidade = $arrEntidade ? $arrEntidade : array();
            
            $htmlDetalhe .= '<tr bgcolor="' . $cor . '" id="tr_' . $key . '">';
            $htmlDetalhe .= '<td>'.$detalhe['emdid'].'</td>';
            $htmlDetalhe .= '<td>'.$v['emecod'].'</td>';
            $htmlDetalhe .= '<td>'.$detalhe['emeid'].'</td>';
            $htmlDetalhe .= '<td>'.$detalhe['gndcod'].'</td>';
            $htmlDetalhe .= '<td>'.$detalhe['foncod'].'</td>';
            $htmlDetalhe .= '<td>'.$detalhe['mapcod'].'</td>';
            $htmlDetalhe .= '<td>'.number_format($detalhe['emdvalor'], '2', ',', '.').'</td>';
            $htmlDetalhe .= '<td>'.$detalhe['emdstatus'].'</td>';
            $htmlDetalhe .= '<td>';
            $htmlDetalhe .= '   <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
            $htmlDetalhe .= '   <tr bgcolor="#bfbfbf">';
            $htmlDetalhe .= '       <td><strong>edeid</strong></td>';
            $htmlDetalhe .= '       <th>enbcnpj</th>';
            $htmlDetalhe .= '       <th>enbnome</th>';
            $htmlDetalhe .= '       <th>edevalor</th>';
            $htmlDetalhe .= '       <th>edestatus</th>';
            $htmlDetalhe .= '       <th>Emenda PAR3</th>';
            $htmlDetalhe .= '   </tr>';
            foreach ($arrEntidade as $ent) {
                $sql = "SELECT distinct ineid, inpid, edeid, edeid_bk, inestatus, inevalor FROM par3.iniciativa_emenda WHERE edeid = {$ent['edeid']} AND emeid = {$v['emeid']} and inestatus = 'A' ORDER BY inestatus, ineid DESC";
                $arrPAR3 = $db->carregar($sql);
                $arrPAR3 = $arrPAR3 ? $arrPAR3 : array();
                
                $htmlDetalhe .= '   <tr>';
                $htmlDetalhe .= '       <td style="color: blue; font-weight: bold;">'.$ent['edeid'].'</td>';
                $htmlDetalhe .= '       <td>'.$ent['enbcnpj'].'</td>';
                $htmlDetalhe .= '       <td>'.$ent['enbnome'].'</td>';
                $htmlDetalhe .= '       <td>'.number_format($ent['edevalor'], '2', ',', '.').'</td>';
                $htmlDetalhe .= '       <td>'.$ent['edestatus'].'</td>';
                $htmlDetalhe .= '       <td>';
                $htmlDetalhe .= '       <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
                $htmlDetalhe .= '       <tr bgcolor="#e6e6ff">';
                $htmlDetalhe .= '           <th>ineid</th>';
                $htmlDetalhe .= '           <th>inpid</th>';
                $htmlDetalhe .= '           <th>edeid_bk</th>';
                $htmlDetalhe .= '           <th>inestatus</th>';
                $htmlDetalhe .= '           <th>inevalor</th>';
                $htmlDetalhe .= '       </tr>';
                foreach ($arrPAR3 as $par) {
                    $htmlDetalhe .= '   <tr>';
                    $htmlDetalhe .= '       <td>'.$par['ineid'].'</td>';
                    $htmlDetalhe .= '       <td>'.$par['inpid'].'</td>';
                    $htmlDetalhe .= '       <td style="color: blue; font-weight: bold;">'.$par['edeid_bk'].'</td>';
                    $htmlDetalhe .= '       <td>'.$par['inestatus'].'</td>';
                    $htmlDetalhe .= '       <td>'.$par['inevalor'].'</td>';
                    $htmlDetalhe .= '   </tr>';
                }
                $htmlDetalhe .= '       </table>';
                $htmlDetalhe .= '       </td>';
                $htmlDetalhe .= '   </tr>';
            }
            $htmlDetalhe .= '   </table>';
            $htmlDetalhe .= '</td>';
            $htmlDetalhe .= '</tr>';
            }
        }
        
        if( $_REQUEST['backup'] == 'S' ){
            foreach ($arrEmendaDetalhe_bk as $detalhe) {
                $sql = "SELECT ede.edeid, eb.enbcnpj, eb.enbnome, ede.edevalor, ede.edestatus FROM emenda.emendadetalheentidade_bk_01 ede
                        	INNER JOIN emenda.entidadebeneficiada eb ON eb.enbid = ede.enbid
                        WHERE ede.emdid = {$detalhe['emdid']} order by eb.enbcnpj";
                $arrEntidade_bk = $db->carregar($sql);
                $arrEntidade_bk = $arrEntidade_bk ? $arrEntidade_bk : array();
                
                $htmlDetalhe_bk .= '<tr bgcolor="' . $cor . '" id="tr_' . $key . '">';
                $htmlDetalhe_bk .= '<td>'.$detalhe['emdid'].'</td>';
                $htmlDetalhe_bk .= '<td>'.$v['emecod'].'</td>';
                $htmlDetalhe_bk .= '<td>'.$detalhe['emeid'].'</td>';
                $htmlDetalhe_bk .= '<td>'.$detalhe['gndcod'].'</td>';
                $htmlDetalhe_bk .= '<td>'.$detalhe['foncod'].'</td>';
                $htmlDetalhe_bk .= '<td>'.$detalhe['mapcod'].'</td>';
                $htmlDetalhe_bk .= '<td>'.number_format($detalhe['emdvalor'], '2', ',', '.').'</td>';
                $htmlDetalhe_bk .= '<td>'.$detalhe['emdstatus'].'</td>';
                $htmlDetalhe_bk .= '<td>';
                $htmlDetalhe_bk .= '   <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
                $htmlDetalhe_bk .= '   <tr bgcolor="#bfbfbf">';
                $htmlDetalhe_bk .= '       <th>edeid</th>';
                $htmlDetalhe_bk .= '       <th>enbcnpj</th>';
                $htmlDetalhe_bk .= '       <th>enbnome</th>';
                $htmlDetalhe_bk .= '       <th>edevalor</th>';
                $htmlDetalhe_bk .= '       <th>edestatus</th>';
                $htmlDetalhe_bk .= '       <th>Emenda PAR3</th>';
                $htmlDetalhe_bk .= '   </tr>';
                foreach ($arrEntidade_bk as $ent) {
                    $sql = "SELECT distinct ineid, inpid, edeid, edeid_bk, inevalor, inestatus FROM par3.iniciativa_emenda_bk_01 WHERE edeid = {$ent['edeid']} AND emeid = {$v['emeid']} and inestatus = 'A' ORDER BY ineid DESC";
                    $arrPAR3_bk = $db->carregar($sql);
                    $arrPAR3_bk = $arrPAR3_bk ? $arrPAR3_bk : array();
                    
                    $htmlDetalhe_bk .= '   <tr>';
                    $htmlDetalhe_bk .= '       <td style="color: blue; font-weight: bold;">'.$ent['edeid'].'</td>';
                    $htmlDetalhe_bk .= '       <td>'.$ent['enbcnpj'].'</td>';
                    $htmlDetalhe_bk .= '       <td>'.$ent['enbnome'].'</td>';
                    $htmlDetalhe_bk .= '       <td>'.number_format($ent['edevalor'], '2', ',', '.').'</td>';
                    $htmlDetalhe_bk .= '       <td>'.$ent['edestatus'].'</td>';
                    $htmlDetalhe_bk .= '       <td>';
                    $htmlDetalhe_bk .= '       <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
                    $htmlDetalhe_bk .= '       <tr bgcolor="#e6e6ff">';
                    $htmlDetalhe_bk .= '           <th>ineid</th>';
                    $htmlDetalhe_bk .= '           <th>inpid</th>';
                    $htmlDetalhe_bk .= '           <th>edeid_bk</th>';
                    $htmlDetalhe_bk .= '           <th>inestatus</th>';
                    $htmlDetalhe_bk .= '           <th>inevalor</th>';
                    $htmlDetalhe_bk .= '       </tr>';
                    foreach ($arrPAR3_bk as $par) {
                        $htmlDetalhe_bk .= '   <tr>';
                        $htmlDetalhe_bk .= '       <td>'.$par['ineid'].'</td>';
                        $htmlDetalhe_bk .= '       <td>'.$par['inpid'].'</td>';
                        $htmlDetalhe_bk .= '       <td style="color: blue; font-weight: bold;">'.$par['edeid_bk'].'</td>';
                        $htmlDetalhe_bk .= '       <td>'.$par['inestatus'].'</td>';
                        $htmlDetalhe_bk .= '       <td>'.$par['inevalor'].'</td>';
                        $htmlDetalhe_bk .= '   </tr>';
                    }
                    $htmlDetalhe_bk .= '       </table>';
                    $htmlDetalhe_bk .= '       </td>';
                    $htmlDetalhe_bk .= '   </tr>';
                }
                $htmlDetalhe_bk .= '   </table>';
                $htmlDetalhe_bk .= '</td>';
                $htmlDetalhe_bk .= '</tr>';
            }
        }
    //}
}

$html = '<table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
$html .= '<tr>';
if( $_REQUEST['original'] == 'S' ){
$html .= '  <td valign="top">';
$html .= '       <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
$html .= '      <tr>';
$html .= '          <th colspan=10>Emenda Detalhe</th>';
$html .= '      </tr>';
$html .= '      <tr bgcolor="#e6e6e6">';
$html .= '          <th>emdid</th>';
$html .= '          <th>emecod</th>';
$html .= '          <th>emeid</th>';
$html .= '          <th>gnd</th>';
$html .= '          <th>fon</th>';
$html .= '          <th>map</th>';
$html .= '          <th>emdvalor</th>';
$html .= '          <th>emdstatus</th>';
$html .= '          <th>Emenda Entidade</th>';
$html .= '      </tr>';
$html .=            $htmlDetalhe;
$html .= '      </table>';
$html .= '  </td>';
}
if( $_REQUEST['backup'] == 'S' ){
$html .= '  <td valign="top">';
$html .= '       <table align="center" class="listagem" width="100%" border="1" cellspacing="0" cellpadding="2">';
$html .= '      <tr>';
$html .= '          <th colspan=10>Emenda Detalhe Backup</th>';
$html .= '      </tr>';
$html .= '      <tr bgcolor="#e6e6e6">';
$html .= '          <th>emdid</th>';
$html .= '          <th>emecod</th>';
$html .= '          <th>emeid</th>';
$html .= '          <th>gnd</th>';
$html .= '          <th>fon</th>';
$html .= '          <th>map</th>';
$html .= '          <th>emdvalor</th>';
$html .= '          <th>emdstatus</th>';
$html .= '          <th>Emenda Entidade</th>';
$html .= '      </tr>';
$html .=            $htmlDetalhe_bk;
$html .= '      </table>';
$html .= '  </td>';
}
$html .= '</tr>';
$html .= '</table>';
echo $html;
//ver(simec_htmlentities($html));
    
    /*$sql = "SELECT emdid, emeid, gndcod, foncod, mapcod, emdvalor, emddataalteracao, usucpfalteracao, emdtipo, emdimpositiva, emdtipodescentralizacao, emdliberacaosri, emddtultimoenviosiop, emdstatus, emddotacaoinicial
            FROM emenda.emendadetalhe_bk_01 where emeid = {$v['emeid']}";
    $arrEmendaDetalhe = $db->carregar($sql);
    $arrEmendaDetalhe = $arrEmendaDetalhe ? $arrEmendaDetalhe : array();
    
    foreach ($arrEmendaDetalhe as $detalhe) {
        
    }
    
    $sql = "SELECT ineid, inpid, edeid, emeid, inevalor, inestatus, docid, ineresponsavel, inecontrapartida, inedata, usucpf, edeid_bk
        FROM par3.iniciativa_emenda_bk_01 where edeid = ";
    $arrItem = $db->carregar($sql);
    $arrItem = $arrItem ? $arrItem : array();*/
    

