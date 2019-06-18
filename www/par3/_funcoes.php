<?php
require_once APPRAIZ. '/www/par3/programas/_funcoes_programa_mais_alfabetizacao.php';

function verifica_preenchimento_bandalarga($docid)
{
    global $db;

    $sql = "SELECT DISTINCT
            	inuid
            FROM
            	par3.questionario_unidade
            WHERE
            	docid = $docid";

    $inuid = $db->pegaUm($sql);

    $modelBandaLarga = new Par3_Model_Relatorio_BandaLarga();

    return $modelBandaLarga->carregarQtdRespostasMunicipio($inuid) >= 20;
}

function rerifica_vigencia_questionário($docid)
{
    global $db;

    $modelBandaLarga = new Par3_Model_Relatorio_BandaLarga();
    return $modelBandaLarga->recuperaQuetionarioVigente();
}

function carregarPerfisResp()
{
    global $db;

    $sql = "SELECT DISTINCT
            	pfl.pflcod
            FROM
            	seguranca.perfil pfl
            INNER JOIN par3.tprperfil trp ON trp.pflcod = pfl.pflcod
            WHERE
            	pfl.sisid = 231
            	AND pfl.pflstatus = 'A'";

    return $db->carregarColuna($sql);
}

function checaPerfilValido($arrPerfilValido, $arrPerfil)
{
    $retorno = 'N';
    foreach ($arrPerfilValido as $perfil) {
        if (in_array($perfil, $arrPerfil)) {
            $retorno = 'S';
        }
    }
    return $retorno;
}


function possuiPerfil($pflcods)
{

    global $db;

    if ($db->testa_superuser()) {
        return true;
    }

    if (is_array($pflcods)) {
        $pflcods = array_map("intval", $pflcods);
        $pflcods = array_unique($pflcods);
    } else {
        $pflcods = array((integer)$pflcods);
    }
    if (count($pflcods) == 0) {
        return false;
    }
    $sql = "SELECT
					count(*)
			FROM seguranca.perfilusuario
			WHERE
				usucpf = '" . $_SESSION['usucpf'] . "' and
				pflcod in ( " . implode(",", $pflcods) . " ) ";
    return $db->pegaUm($sql) > 0;
}

function pegaResponssabilidade($tprcod)
{

    global $db;

    if ($tprcod == '1') {//Lista Estados
        $sql = "SELECT
					ur.estuf
				FROM
					par3.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A'";
    } elseif ($tprcod == '2') {//Lista Município
        $sql = "SELECT
					ur.muncod
				FROM
					par3.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '" . $_SESSION['usucpf'] . "' AND
					ur.rpustatus = 'A'";
    }

    $r = $db->carregarColuna($sql);

    return $r;
}

function pegaPerfils($usucpf)
{

    global $db;

    $sql = "SELECT
				pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";

    $pflcod = $db->carregarColuna($sql);
    return $pflcod;
}

function pegarPerfilComResponsabilidadePorMenu($usucpf)
{
    global $db;
    
    $sql = "select
                distinct tp.pflcod
            from
                seguranca.perfilusuario pu
            join par3.tprperfil tp on tp.pflcod = pu.pflcod
            join seguranca.perfil p on p.pflcod = tp.pflcod and
                                       p.pflstatus = 'A'
            join seguranca.perfilmenu pm on pm.pflcod = p.pflcod and
            								pm.pmnstatus = 'A' and
                                            pm.mnuid = {$_SESSION['mnuid']}
            where
                p.sisid = {$_SESSION['sisid']} and
                pu.usucpf = '{$usucpf}'";
    $arDado = $db->carregarColuna($sql);
    
    return ($arDado ? $arDado : array());
}

function pegaPerfilUsuarioPorMenu($usucpf)
{
    global $db;
    
    $sql = "SELECT
                DISTINCT
            	pu.pflcod
            FROM
            	seguranca.perfil p
            JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod
            JOIN seguranca.perfilmenu pm ON pm.pflcod = pu.pflcod and
            								pm.pmnstatus = 'A' and
                                            pm.mnuid = {$_SESSION['mnuid']}
            WHERE
            	p.sisid = '{$_SESSION['sisid']}'
            	AND pu.usucpf = '{$usucpf}'";
    
    return $db->carregarColuna($sql);
}

function pegarResponsabilidadeUsuario($usucpf)
{
    global $db;

    $arPerfil                    = pegaPerfilUsuarioPorMenu($usucpf);
    $arPerfilComResponsabilidade = pegarPerfilComResponsabilidadePorMenu($usucpf);
    $arPerfilDiff                = array_diff($arPerfil, $arPerfilComResponsabilidade);
    // Padrão: possui perfil sem responsabilidade $arResponsabilidade = false
    $arResponsabilidade = false;
    // Quando o perfil só possui perfis com RESPONSABILIDADE
    if (count($arPerfilDiff) == 0 && count($arPerfilComResponsabilidade) > 0) {
        $arResponsabilidade = array();
        $sql = "SELECT
                    distinct
					ur.estuf
				FROM
					par3.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '{$usucpf}' AND
					ur.pflcod IN (". implode(", ", $arPerfilComResponsabilidade) .") AND
					ur.rpustatus = 'A' AND
                    ur.estuf IS NOT NULL";
        $estuf = $db->carregarColuna($sql);
        
        $sql = "SELECT
                    distinct
					ur.muncod
				FROM
					par3.usuarioresponsabilidade ur
				WHERE
					ur.usucpf = '{$usucpf}' AND
					ur.pflcod IN (". implode(", ", $arPerfilComResponsabilidade) .") AND
					ur.rpustatus = 'A' AND
                    ur.muncod IS NOT NULL";
        $muncod = $db->carregarColuna($sql);
        // Se tiver alguma responsabilidade atribuída
        if (count($estuf) > 0 || count($muncod) > 0) {
            $arResponsabilidade['estuf'] = $estuf;
            $arResponsabilidade['muncod'] = $muncod;
        // Se só tiver perfil com RESPONSABILIDADE, porém sem nenhuma atruibuição
        } else {
            $arResponsabilidade['sem_responsabilidade_atribuida'] = true;
        }
    }
    
    return $arResponsabilidade;
}

function pegaPerfil($usucpf)
{

    global $db;

    $sql = "SELECT
				pu.pflcod
			FROM seguranca.perfil AS p
			LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
			WHERE
				p.sisid = '{$_SESSION['sisid']}'
				AND pu.usucpf = '$usucpf'";

    $pflcod = $db->pegaUm($sql);
    return $pflcod;
}

function criaAbaGuia()
{
    $abasGuia = array(
        0 => array("descricao" => "Diagnóstico Estadual", "link" => "par3.php?modulo=principal/configuracao/guia&acao=A&itrid=1"),
        1 => array("descricao" => "Diagnóstico Municipal", "link" => "par3.php?modulo=principal/configuracao/guia&acao=A&itrid=2"),
        2 => array("descricao" => "Diagnóstico do Distrito Federal", "link" => "par3.php?modulo=principal/configuracao/guia&acao=A&itrid=3")
    );
    return $abasGuia;
}

function criaAbaReformulacao($inuid, $tirid = 2, $refid = "")
{
    if( $refid ){
        $abasGuia = array(
            0 => array("descricao" => "Documentos", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos&inuid=$inuid"),
            1 => array("descricao" => "Solicitar Reprogramação", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}&refid={$_REQUEST['refid']}"),
        );
    } else {
        if( $tipo == 2 ){
            $abasGuia = array(
                0 => array("descricao" => "Documentos", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos&inuid=$inuid"),
                1 => array("descricao" => "Solicitar Reprogramação", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaReprogramacao&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}"),
            );
        } else {
            $abasGuia = array(
                0 => array("descricao" => "Documentos", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento&acao=A&aba=documentos&inuid=$inuid"),
                1 => array("descricao" => "Prorrogação de Prazo", "link" => "par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid={$_REQUEST['dotid']}&inuid={$_REQUEST['inuid']}&proid={$_REQUEST['proid']}"),
            );
            
        }
    }
    return $abasGuia;
}

function criaAbaPar($bEstruturaAvaliacao = false)
{
    if (!$_SESSION['par']['boAbaMunicipio'] && !$_SESSION['par']['boAbaEstado']) {
        $abasPar = array(0 => array("descricao" => "Lista de Estados", "link" => "par3.php?modulo=principal/listaEstados&acao=A"),
                         1 => array("descricao" => "Lista de Municípios", "link" => "par3.php?modulo=principal/listaMunicipios&acao=A"),
                         2 => array("descricao" => "Análise", "link" => "par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarIndicadorAnalise&acao=A"),
                         3 => array("descricao" => "Orçamento", "link" => "par3.php?modulo=principal/orcamento/empenhoPar&acao=A"),
                         4 => array("descricao" => "Emendas", "link" => "par3.php?modulo=principal/emendas/listaEmendas&acao=A")
        );
    } elseif ($_SESSION['par']['boAbaMunicipio']) {
        $abasPar = array(
            0 => array("descricao" => "Lista de Municípios", "link" => "par3.php?modulo=principal/listaMunicipios&acao=A")
        );
    } elseif ($_SESSION['par']['boAbaEstado']) {
        $abasPar = array(
            0 => array("descricao" => "Lista de Estados", "link" => "par3.php?modulo=principal/listaEstados&acao=A")
        );
    }
    return $abasPar;
}

function criaAbaIniciativaAnalise()
{
    $abasPar = array(0 => array("descricao" => "Lista Iniciativa Análise", "link" => "par3.php?modulo=principal/planoTrabalho/planejamento/pesquisarIndicadorAnalise&acao=A"),
                     1 => array("descricao" => "Análise de Iniciativa", "link" => "par3.php?modulo=principal/planoTrabalho/planejamento/analisarPlanejamentoIniciativa&acao=A"),
                    );

    return $abasPar;
}

function criaAbaModeloDocumento()
{
    $abasPar = array(0 => array("descricao" => "Lista Modelo", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=modelodocumento"),
                     1 => array("descricao" => "Gerar Modelo", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=gerarmodelodocumento"),
                    );

    return $abasPar;
}

function criaAbaRegrasTermos()
{
    $abasPar = array(0 => array("descricao" => "Configuração", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=listar_regra_termo"),
                     1 => array("descricao" => "Cadastrar", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastrar_regra_termo"),
                    );

    return $abasPar;
}

function criaAbaConfigurarTelas()
{
    $abasPar = array(0 => array("descricao" => "Configuração", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=configurarTelasPerfil"),
                     1 => array("descricao" => "Cadastrar", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastrarConfiguracaoTela"),
                    );

    return $abasPar;
}

function criaAbaPainelAviso()
{
    $abasPar = array(0 => array("descricao" => "Lista de Aviso", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=aviso"),
                     1 => array("descricao" => "Cadastro de Aviso", "link" => "par3.php?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=cadastroAviso"),
                    );

    return $abasPar;
}

function criaAbaPagamento()
{
    $abasPar = array(0 => array("descricao" => "Lista de Pagamentos Solicitados", "link" => "par3.php?modulo=principal/orcamento/listaPagamentos&acao=A"),
                     1 => array("descricao" => "Solicitar Pagamento FNDE", "link" => "par3.php?modulo=principal/orcamento/solicitarPagamento&acao=A"),
                    );

    return $abasPar;
}

function criaAbaIndicadoresQualitativos($inuid, $arDimensoes)
{

    $abasIQ = array();
    $d = 0;
    $descricao = "";

    if (is_array($arDimensoes)) {
        foreach ($arDimensoes as $dimensao) {
            $d++;
            $percentualAba = recuperaSituacaoPreenchimentoDiagnostico($inuid, $dimensao['dimid']);
            $descricao = '
								' . $dimensao['dimcod'] . '. ' . (strlen($dimensao['dimdsc']) >= 27 ? substr($dimensao['dimdsc'], 0, 27) . "..." : $dimensao['dimdsc']) . '&nbsp;
								<span class="pie">' . number_format($percentualAba['percent'], 0) . '/100 &nbsp;</span>';
            if ($d == 2) :
                $descricao .= '&nbsp;<span style="padding: 4px 0px 4px 4px" class="badge badge-danger tabIndicadoresDemonstrativo"><i class="fa fa-list-ul" style="margin: 0px 4px 0px 0.5px" data-toggle="modal" data-target="#quadroDemonstrativo"></i></span>';
            endif;
            if ($d == 4) :
                $descricao .= '&nbsp;<span style="padding: 4px 0px 4px 4px" class="badge badge-danger tabIndicadoresGrafico"><i class="fa fa-list-ul" style="margin: 0px 4px 0px 0px" data-toggle="modal" data-target="#graficoIndicadores"></i></span>';
            endif;

            array_push($abasIQ, array("descricao" => $descricao, "link" => "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=" . $inuid . "&dimid=" . $dimensao['dimid']));
        }
    }

    $descricaoSintese = '<span class="tabIndicadores">Síntese do Diagnóstico</span>';
    array_push($abasIQ, array("descricao" => $descricaoSintese, "link" => "par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=" . $inuid . "&resumo=1"));
    return $abasIQ;
}

function pegaArrayPerfil($usucpf)
{

    global $db;

    $sql = "SELECT
	pu.pflcod
	FROM
	seguranca.perfil AS p
	LEFT JOIN seguranca.perfilusuario AS pu ON pu.pflcod = p.pflcod
	WHERE
	p.sisid = '{$_SESSION['sisid']}'
	AND pu.usucpf = '$usucpf'";

    $pflcod = $db->carregarColuna($sql);

    return $pflcod;
}

function formata_Tipo_Documento($str)
{
    return '<span class="termo_detalhe">' . $str . '</span>';
}

function formata_Tipo_Documento_Manifesto($str)
{
    return '<span class="termo_detalhe_manifesto">' . $str . '</span>';
}

function formata_Tipo_DocumentoPar2($str)
{
    return '<span class="termo_detalhe_par2" data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="bottom" data-content="Clique para exibir o documento">' . $str . '</span>';
}


function formata_Tipo_Texto_Tabela($str)
{
    return '<div class="text-left">' . $str . '</div>';
}

function formata_Tipo_Numero_Tabela($str)
{
    return '<div class="text-right">' . $str . '</div>';
}

function formata_numero_processo($str)
{
    global $db;
    
    $num = somenteNumeros($str);
    $str = $num ? substr($str, 0, 5) . '.' . substr($str, 5, 6) . '/' . substr($str, 11, 4) . '-' . substr($str, 15, 2) : $str;
    if ($str =='') {
        return '';
    }
    
    $sql = "SELECT count(proid) FROM par3.processo WHERE pronumeroprocesso = '".$num."' AND ungid IS NOT NULL";
    $boExecutora = $db->pegaUm($sql);
    
    if( (int)$boExecutora > (int)0 ){
        return '<span class="executora_detalhe" id="teste_2" data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="right" data-content="Clique para visualizar informações da executora" processo_num="'.$num.'"><i style="color: #64a0e8;" class="fa fa-info-circle class_dropdown"></i></span>
            <span class="processo_detalhe" data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="right" data-content="Clique para visualizar informações do processo">' . $str . '</span>';
    } else {
        return '<span class="processo_detalhe" data-toggle="popover" data-html="true" title="" data-trigger="hover" data-placement="right" data-content="Clique para visualizar informações do processo">' . $str . '</span>';
    }

}


function formata_numero_processo_component($str)
{
    $path = APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';
    if(!in_array($path,get_included_files())){
        require_once $path;
    }
    $num = somenteNumeros($str);
    $str = $num ?
        substr($str, 0, 5) . '.' .
        substr($str, 5, 6) . '/' .
        substr($str, 11, 4) . '-' .
        substr($str, 15, 2)
    : $str;
    if ($str =='') {
        return '';
    }
    return <<<HTML
    <span
    class="processo_detalhe"
    data-toggle="popover"
    data-html="true"
    title=""
    data-trigger="hover"
    data-placement="right"
    data-content="Clique para visualizar informações do processo">{$str}</span>
HTML;
}

function formata_numero_processo_sem_html($str)
{
    $num = somenteNumeros($str);
    $str = $num ? substr($str, 0, 5) . '.' . substr($str, 5, 6) . '/' . substr($str, 11, 4) . '-' . substr($str, 15, 2) : $str;
    if ($str =='') {
        return '';
    }
    return $str;
}

function formata_codigo_solicitacao($numeroSolicitacao, $linha)
{
    $k = 6;
    $valor = strrev($numeroSolicitacao);
    for($i = $k-1; $i>=0; $i--) {
        $mascarado .= (isset($valor[--$k])) ? $valor[$k] : 0;
    }
    return $linha['ano'].$mascarado;
}

$pathDetalhe = APPRAIZ . 'par3/modulos/principal/detalheProcesso.inc';

function checkbox_aprovar_em_lote($obrcheck)
{
    return <<<HTML
    <div class="checkbox checkbox-primary">
    <input type="checkbox" name="obrcheck" id="obrcheck{$obrcheck}" value="{$obrcheck}" />
    <label for="obrcheck{$obrcheck}"></label>
    </div>
HTML;
//    return '<input type="checkbox" name="obrcheck" id="obrcheck'.$obrcheck.'" value="'.$obrcheck.'" />';
}

function btnEditarObraListaDeObras($arrayDados)
{
    $arrayDados = explode('#%#', $arrayDados);
    global $db;
    $possuiEmenda = $db->pegaUm("SELECT count(*) as count FROM par3.iniciativa_emenda_obra ieo inner join par3.iniciativa_emenda ine using(ineid)
                                 where ieo.ieostatus = 'A' and ieo.ieovalor > 0 and obrid = {$arrayDados[0]} and ine.inestatus = 'A' ", 'count');
    $labelEmenda = '';
    if ($possuiEmenda) {
        $labelEmenda = '<span style="color: green;">(EP)</span>&nbsp';
    }
    return '<a
    href="/par3/par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid='.$arrayDados[1].'&inpid='.$arrayDados[2].'&obrid='.$arrayDados[0].'&toReturn=formListaObra"
    title="Clique para acessar a obra">'.$labelEmenda.''.$arrayDados[4].'</a>';
}


function btnEditarObraPlanejamento($arrayDados)
{
    // obra.obrid||'#%#'||obra.inuid||'#%#'||obra.inpid||'#%#'||obra.obrid||'#%#'||obra.obrdsc
    $arrayDados = explode('#%#', $arrayDados);
    global $db;
    $possuiEmenda = $db->pegaUm("SELECT count(*) as count FROM par3.iniciativa_emenda_obra ieo inner join par3.iniciativa_emenda ine using(ineid)
                                 where ieo.ieostatus = 'A' and ieo.ieovalor > 0 and obrid = {$arrayDados[0]} and ine.inestatus = 'A' ", 'count');
    $labelEmenda = '';
    if ($possuiEmenda) {
        $labelEmenda = '<span style="color: green;">(EP)</span>&nbsp';
    }
    return '<a
    href="/par3/par3.php?modulo=principal/planoTrabalho/obras&acao=A&menu=dados_terreno&inuid='.$arrayDados[1].'&inpid='.$arrayDados[2].'&obrid='.$arrayDados[0].'&toReturn=formListaObraPlanejamento"
    title="Clique para acessar a obra">'.$labelEmenda.''.$arrayDados[4].'</a>';
}


function linkObras2($arrayDados)
{
    $arrayDados = explode('#%#', $arrayDados);
    if (!$arrayDados[0]) {
        return ' - ';
    }
    $estadoAtual = wf_pegarEstadoAtual($arrayDados[1]);
    return '<a href="'.$_SERVER['HTTP_ORIGIN'].$_SERVER['REQUEST_URI'].'&requisicao=acessarobras2&obrid='.$arrayDados[0].'" target="_blank" title="ID: '.$arrayDados[0].' - Clique para acessar no Obras 2.0">'.$estadoAtual['esddsc'].'</a>';
}

function popOverSituacaoObra($docid)
{
    if (!$docid) {
        return '';
    }
    include_once APPRAIZ ."includes/workflow.php";
    $analista    = wf_pegarUltimoUsuarioModificacao($docid);
    $data        = wf_pegarUltimaDataModificacao($docid);
    $estadoAtual = wf_pegarEstadoAtual($docid);
    $comentario  = wf_pegarComentarioEstadoAtual($docid);
    $content  = '<p><b>Responsável: </b>'.$analista['usunome'].'</p>';
    $content .= '<p><b>Data: </b>'.$data.'</p>';
    $content .= '<p><b>Comentário: </b>'.($comentario).'</p>';
    return '<a data-toggle="popover" onclick="mostrarHistoricoSituacao('.$docid.')" class="popoversituacao" data-html="true" title="'.$estadoAtual['esddsc'].'" data-trigger="hover" data-content=\''.$content.'\' data-placement="auto right">'.$estadoAtual['esddsc'].'</a>';
}

function popOverJustificativaReformulacao($texto, $arrDados)
{
    //ver($texto, $arrDados['justificativa_completa'],d);
    $content .= '<p style="text-align: justify;">'.$arrDados['justificativa_completa'].'</p>';
    return '<a data-toggle="popover" class="popoversituacao" data-html="true" title="Justificativa" data-trigger="hover" data-content=\''.$content.'\' data-placement="auto right">'.$arrDados['refjustificativa'].'</a>';
}

function popOverEmendas($anaid)
{
    global $db;

    if (!empty($anaid)) {
        $sql = "SELECT a.edeid, ve.emecod, it.intaid, it.intadsc, au.autcod||' - '||au.autnome AS parlamentar
                FROM par3.analise a
                	LEFT JOIN par3.iniciativa_tipos_assistencia it ON it.intaid = a.intaid
                	LEFT JOIN emenda.v_emendadetalheentidade ve
                        INNER JOIN emenda.autor au ON au.autid = ve.autid
                    ON ve.edeid = a.edeid
                WHERE anaid = $anaid";
        $arrDados = $db->pegaLinha($sql);

        if ($arrDados['intaid'] == 2) {
            $content  = '<p><b>Código: </b>'.$arrDados['emecod'].'</p>';
            $content .= '<p><b>Parlamentar: </b>'.$arrDados['parlamentar'].'</p>';
            return '<span data-toggle="popover" data-html="true" title="Emenda" data-trigger="hover" data-placement="left" data-content="'.$content.'">'.$arrDados['intadsc'].'</span>';
        } else {
            return 'PAR';
        }
    } else {
        return 'PAR';
    }
}

function popOverPendenciarsPar($inuid)
{
    global $db;
    
    if (!empty($inuid)) {
        $sql = "SELECT
                	CASE WHEN obras_par = 't' THEN 'Sim' ELSE 'Não' END AS obras_par,
                	CASE WHEN cacs = 't' THEN 'Sim' ELSE 'Não' END AS cacs,
                	CASE WHEN pne = 't' THEN 'Sim' ELSE 'Não' END AS pne,
                	CASE WHEN siope = 't' THEN 'Sim' ELSE 'Não' END AS siope,
                	CASE WHEN habilitacao = 't' THEN 'Sim' ELSE 'Não' END AS habilitacao,
                    CASE WHEN contas = 't' THEN 'Sim' ELSE 'Não' END AS contas
                FROM par3.vm_relatorio_quantitativo_pendencias
                WHERE inuid = $inuid";
        $arPendencia = $db->pegaLinha($sql);
        
        if ($arPendencia['obras_par'] == 'Sim' || $arPendencia['cacs'] == 'Sim' || $arPendencia['pne'] == 'Sim' || $arPendencia['siope'] == 'Sim' || $arPendencia['habilitacao'] == 'Sim' || $arPendencia['contas'] == 'Sim') {
            $content  = '<p><b>Obras: </b>'.$arPendencia['obras_par'].'</p>';
            $content .= '<p><b>CACS: </b>'.$arPendencia['cacs'].'</p>';
            $content .= '<p><b>PNE: </b>'.$arPendencia['pne'].'</p>';
            $content .= '<p><b>SIOPE: </b>'.$arPendencia['siope'].'</p>';
            $content .= '<p><b>Habilita: </b>'.$arPendencia['habilitacao'].'</p>';
            $content .= '<p><b>Contas: </b>'.$arPendencia['contas'].'</p>';
            return '<span data-toggle="popover" data-html="true" title="Pendências" data-trigger="hover" data-placement="left" data-content="'.$content.'" style="color: red"><b>Sim</b></span>';
        } else {
            return '<span style="color: blue"><b>Não</b></span>';
        }
    } else {
        return '<span style="color: blue"><b>Não</b></span>';
    }
}

function remove_acentos($str, $replace = true)
{
    $str = trim($str);
    $str = strtr(
        $str,
        "¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ!@#%&*()[]{}+=?",
        "YuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy_______________"
    );
    if (!$replace) {
        $str = str_replace("..", "", str_replace("/", "", str_replace("\\", "", str_replace("\$", "", $str))));
        return $str;
    } else {
        return str_replace(array(' ', ':'), array('-', ''), strtolower($str));
    }
}

/*
 * Função que retorna a pontuação de uma entidade no PAR
 *
 * O retorno dela é no formato Array('id', 'total', 'pontuacao'):
 * - Id é o dimid, areid ou indid;
 * - Total é o máximo que poderia ser pontuado naquela(e) dimensão/área/indicador;
 * - Pontuacao é a quantidade pontuada pela entidade
 *
 * Autor: Victor McBenzi
 */
function recuperaPontuacaoDiagnostico($inuid, $dimid = null, $areid = null, $indid = null)
{
    global $db;

    if (!$inuid || (!$dimid && !$areid && !$indid)) {
        return false;
    }

    // Dimensao
    if ($dimid) {
        return recuperaPontuacaoDimensaoDiagnostico($inuid, $dimid);
    }

    // Área
    if ($areid) {
        return recuperaPontuacaoAreaDiagnostico($inuid, $areid);
    }

    // Indicador
    if ($indid) {
        return recuperaPontuacaoIndicadorDiagnostico($inuid, $indid);
    }
}

function recuperaPontuacaoDimensaoDiagnostico($inuid, $dimid)
{
    global $db;

    $sql = "SELECT
            	dim.dimid,
            	are.areid
            FROM
            	par3.dimensao dim
            INNER JOIN par3.area are ON are.dimid = dim.dimid AND are.arestatus = 'A'
            WHERE
            	dim.dimstatus = 'A' AND
            	dim.dimid = {$dimid}";

    $Areas = $db->carregar($sql);

    $retornoDimensao['dimid'] = $dimid;
    $retornoDimensao['total'] = 0;
    $retornoDimensao['pontuacao'] = 0;
    if (is_array($Areas)) {
        foreach ($Areas as $area) {
            $dadoRetorno = recuperaPontuacaoAreaDiagnostico($inuid, $area['areid']);
            $retornoDimensao['total'] = $retornoDimensao['total'] + $dadoRetorno['total'];
            $retornoDimensao['pontuacao'] = $retornoDimensao['pontuacao'] + $dadoRetorno['pontuacao'];
        }
    }

    return $retornoDimensao;
}

function recuperaPontuacaoAreaDiagnostico($inuid, $areid)
{
    global $db;

    $sql = "SELECT DISTINCT
            	are.areid,
            	are.aredsc,
            	ind.indid
            FROM
            	par3.area are
            LEFT JOIN par3.indicador ind ON ind.areid = are.areid AND ind.indstatus = 'A'
            WHERE
            	are.arestatus = 'A' AND
            	are.areid = {$areid}";

    $indicadores = $db->carregar($sql);

    $retornoArea['areid'] = $areid;
    $retornoArea['total'] = 0;
    $retornoArea['pontuacao'] = 0;

    $semCriterios = contarIndicadoresSemCriterio($inuid, $areid);

    if (is_array($indicadores)) {
        foreach ($indicadores as $indicador) {
            if ($indicador['indid']) {
                $arrDadoRetorno = recuperaPontuacaoIndicadorDiagnostico($inuid, $indicador['indid']);
                foreach ($arrDadoRetorno as $dadoRetorno) {
                    $indicadorSemCriterios = contarIndicadoresSemCriterio($inuid, null, $indicador['indid']);
                    $retornoArea['total'] = $retornoArea['total'] + $dadoRetorno['total'];
                    $retornoArea['pontuacao'] = $retornoArea['pontuacao'] + $dadoRetorno['pontuacao'];
                    $retornoArea['aredsc'] = $indicador['aredsc'];
                }
            }
        }
    }

    $retornoArea['total'] = contarIndicadoresSemCriterio($inuid, $areid, null);

    return $retornoArea;
}

function recuperaPontuacaoIndicadorDiagnostico($inuid, $indid)
{
    global $db;

    $sql = "SELECT 	foo.indid, sum(foo.total) as total,
					sum(foo.pontuacao) as pontuacao,
					foo.ptcpontuacao as percentual
				FROM (
                SELECT
                    ind.indid,
                	COALESCE(count(crt.crtid), 0) as total,
                	CASE WHEN dim.dimcod IN (1,2,3) THEN
                	    COALESCE(count(ptcid), 0)
                	ELSE
                	   CASE WHEN (ptcpontuacao < 50 ) THEN COALESCE(sum(0), 0)
                		 WHEN  (ptcpontuacao > 49 AND ptcpontuacao < 85 ) THEN COALESCE(sum(0.5), 0)
                		 WHEN  (ptcpontuacao > 84 ) THEN COALESCE(sum(1), 0)
                	   END
                	END as pontuacao,
                	ptcpontuacao
                FROM
                    par3.indicador ind
                INNER JOIN par3.area are ON are.areid = ind.areid AND are.arestatus = 'A'
                INNER JOIN par3.dimensao dim ON dim.dimid = are.dimid AND dim.dimstatus = 'A'
                INNER JOIN par3.criterio crt ON crt.indid = ind.indid AND crt.crtstatus = 'A'
                LEFT  JOIN par3.pontuacao pto ON pto.indid = ind.indid AND pto.inuid = {$inuid}
                LEFT  JOIN par3.pontuacaocriterio ptc ON ptc.ptoid = pto.ptoid AND ptc.crtid = crt.crtid
                WHERE
                	ind.indstatus = 'A' AND
                	ind.indid = {$indid} AND
                	crt.crtid NOT IN ( SELECT crtidpai FROM par3.criterio_vinculacao WHERE crvvinculo = 2 )
                GROUP BY
    	           ind.indid, dim.dimcod, ptcpontuacao
            ) as foo GROUP BY foo.indid, foo.ptcpontuacao";

    return $db->carregar($sql);
}

function recuperaPontuacaoCriteriosDiagnostico($inuid, $indid)
{
    global $db;

    $sql = "SELECT
	            ptc.ptcpontuacao as percentual, ind.indid,
                crt.crtid NOT IN ( SELECT crtidpai FROM par3.criterio_vinculacao WHERE crvvinculo = 2 ) as vinculo
            FROM par3.criterio crt
            INNER JOIN par3.indicador           ind ON ind.indid = crt.indid
            INNER JOIN par3.area                are ON are.areid = ind.areid AND are.arestatus = 'A'
            INNER JOIN par3.dimensao            dim ON dim.dimid = are.dimid AND dim.dimstatus = 'A'
            LEFT  JOIN par3.pontuacao           pto ON pto.indid = ind.indid AND pto.inuid = {$inuid}
            LEFT  JOIN par3.pontuacaocriterio   ptc ON ptc.ptoid = pto.ptoid AND ptc.crtid = crt.crtid
            WHERE
                crt.crtstatus = 'A'
                AND ind.indstatus = 'A'
                AND ind.indid = {$indid}
            	AND crt.crtid NOT IN (SELECT v.crtid
            				FROM par3.criterio_vinculacao v
            				INNER JOIN par3.pontuacaocriterio   ptc ON ptc.crtid = v.crtidpai
            				WHERE crvvinculo = 2)
                AND crt.crtid NOT IN (SELECT crtidpai FROM par3.criterio_vinculacao WHERE crvvinculo = 2)";

    return $db->carregar($sql);
}

function contarIndicadoresSemCriterio($inuid, $areid = null, $indid = null)
{
    global $db;

    if ($areid) {
        $and = " AND are.areid = {$areid}";
    } else {
        $and = " AND ind.indid = {$indid}";
    }

    $sql = "SELECT COUNT(DISTINCT crt.crtid)
            FROM par3.criterio crt
            INNER JOIN par3.indicador ind ON ind.indid = crt.indid
            INNER JOIN par3.area are ON are.areid = ind.areid AND are.arestatus = 'A'
            INNER JOIN par3.dimensao dim ON dim.dimid = are.dimid AND dim.dimstatus = 'A'
            LEFT  JOIN par3.pontuacao pto ON pto.indid = ind.indid AND pto.inuid = $inuid
            LEFT  JOIN par3.pontuacaocriterio ptc ON ptc.ptoid = pto.ptoid AND ptc.crtid = crt.crtid
            WHERE
            	crt.crtstatus = 'A'
            	AND ind.indstatus = 'A'
            	$and
            	AND crt.crtid NOT IN ( SELECT crtidpai FROM par3.criterio_vinculacao WHERE crvvinculo = 2 )
            	AND crt.crtid NOT IN (SELECT v.crtid
            				FROM par3.criterio_vinculacao v
            				INNER JOIN par3.pontuacaocriterio   ptc ON ptc.crtid = v.crtidpai
            				WHERE crvvinculo = 2)";

    return $db->pegaUm($sql);
}

function recuperaSituacaoPreenchimentoDiagnostico($inuid, $dimid = null)
{
    global $db;

    $dimensao = $dimid ? " AND dim.dimid = {$dimid} " : "";

    $sql = "SELECT
                iu.itrid
            FROM
                par3.instrumentounidade iu
            WHERE
                iu.inuid = {$inuid}";

    $itrid = $db->pegaUm($sql);

    $sql = "SELECT DISTINCT
                are.areid
            FROM
                par3.dimensao dim
            INNER JOIN par3.area are ON are.dimid = dim.dimid AND are.arestatus = 'A'
            WHERE
                dim.dimstatus = 'A' AND
                dim.itrid = {$itrid}
                {$dimensao}
            ORDER BY
                are.areid";

    $Areas = $db->carregarColuna($sql);

    $oAreaController = new Par3_Controller_Area();

    $result = array();

    if (is_array($Areas)) {
        foreach ($Areas as $area) {
            $dadosPercent = $oAreaController->pontuacaoArea($area, $inuid);
            $indicadores = $indicadores + $dadosPercent['indicadores'];
            $pontuados = $pontuados + $dadosPercent['pontuados'];
        }
        $result['indicadores'] = $indicadores;
        $result['pontuados'] = $pontuados;
        $result['percent'] = $result['indicadores'] > 0 ? ($result['pontuados'] * 100) / $result['indicadores'] : 0;
    }

    return $result;
}

/**
 * Função usada na listagem de Conselheiros CAE como callbackDeCampo
 * @param $arqid
 * @param $linha
 * @return string
 */
function setaLinkArquivo($arqid, $linha)
{
    if ($arqid) {
        $url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=3808&menu=cae&req=downloadAta&arqid=' . $arqid;
        return sprintf('<a href="%s"><i class="fa fa-download"></i>&nbsp;%s</a>', $url, 'Abrir anexo');
    }
}

function checkParamInuid()
{
    if (isset($_GET['inuid']) && is_numeric($_GET['inuid']) && $_GET['inuid'] > 0) {
        return true;
    }

    header('location: /par3/par3.php?modulo=principal/listaMunicipios&acao=A');
}

function validaenvioAnalisePNFCD($inuid)
{
    global $db;

    $sql = "select pppresp0, pppresp1arqid, pppresp1, pppresp2, pppresp3 from par3.proadesao_respostas_pnfcd where inuid = $inuid";
    $arDados = $db->pegaLinha($sql);
    if ($arDados['pppresp0'] == 'A') {
        if (!empty($arDados['pppresp1arqid']) && !empty($arDados['pppresp2']) && !empty($arDados['pppresp3'])) {
            return true;
        } else {
            return 'É necessário preencher as perguntas do formulário.';
        }
    } else {
        if (!empty($arDados['pppresp1']) && !empty($arDados['pppresp2']) && !empty($arDados['pppresp3'])) {
            return true;
        } else {
            return 'É necessário preencher as perguntas do formulário.';
        }
    }
}
function formatar_telefone($telefone)
{
    if (empty($telefone)) {
        return;
    } else {
        return '(' . substr($telefone, 0, 2) . ') ' . substr($telefone, 2, 4) . ' - ' . substr($telefone, 6, strlen($telefone));
    }
}

function formatar_cep($cep)
{
    if (empty($cep)) {
        return;
    } else {
        return substr($cep, 0, 5) . ' - ' . substr($cep, 5, strlen($cep));
    }
}

function colunaPercentualDignoticoListaMunicipios($percentual, $args)
{
    return <<<HTML
        <div class="progress progress-striped active">
            <div style="width: {$percentual}%" aria-valuemax="100" aria-valuemin="0" aria-valuenow="{$percentual}" role="progressbar" class="progress-bar progress-bar-primary">
                <span style="color:#676a6c;">{$percentual}%</span>
            </div>
        </div>
HTML;
}

function colunaPercentualDignoticoListaMunicipiosPlanejamento($esdid)
{
    $percentualCor = '';
    $descricao = '';
    switch ($esdid) {
        case '1874':
            $percentualCor = 'danger';
            $descricao = 'Planejamento Não Iniciado';
            break;
        case '1999':
            $percentualCor = 'primary';
            $descricao = 'Em Elaboração';

            break;
        case '2000':
            $percentualCor = 'success';
            $descricao = 'Enviado para Análise';

            break;
        default:
            $percentualCor = 'danger';
            $descricao = 'Planejamento Não Iniciado';
            break;
    }
    return <<<HTML
        <div class="progress progress-striped active">
            <div style="width:100%" aria-valuemax="100" aria-valuemin="0" aria-valuenow="100" role="progressbar" class="progress-bar progress-bar-{$percentualCor}">
                <span>{$descricao}</span>
            </div>
        </div>
HTML;
}

function addCheckbox($id, $options)
{
    $acao = <<<HTML
        <label class="checkbox-inline">
        	<input value="option1" id="inlineCheckbox1" type="checkbox"> Validado </label> <label class="checkbox-inline">
            <input value="option2" id="inlineCheckbox2" type="checkbox"> Em Diligência </label> <label class="checkbox-inline">
       </label>
HTML;
    return sprintf($acao, $id, $id);
}

function html2Pdf($content, $nomearquivo = "programapar")
{
    global $db;
    /* configurações */
    ini_set("memory_limit", "2048M");
    set_time_limit(0);
    /* FIM configurações */

    //ob_clean();

    // -- Preparando a requisição ao webservice de conversão de HTML para PDF do MEC.
    $content = http_build_query(
        array('conteudoHtml' => ($content))
    );

    $context = stream_context_create(
        array(
            'http' => array(
                'method' => 'POST',
                'content' => $content
            )
        )
    );

    // -- Fazendo a requisição de conversão
    $contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
    header('Content-Type: application/pdf');
    header("Content-Disposition: attachment; filename=" . $nomearquivo . "-" . date("Ymdhis") . ".pdf");
    echo $contents;
    echo "<script>window.location.href = '" . $_SERVER['HTTP_REFERER'] . "';</script>";
    exit();
}

//Tipos de Atendimento
function formata_esfera($teresfera)
{
    switch ($teresfera) {
        case '1':
            return 'Estadual';
            break;
        case '2':
            return 'Municipal';
            break;
        default:
            return $teresfera;
            break;
    }
}

function formata_separados_virgula_xls($str)
{
    return str_replace(",", " - ", $str);
}

//Ciclos PAR
function formata_vigencia($vigencia)
{
    switch ($vigencia) {
        case 'VIGENTE':
            return 'Vigente';
            break;
        case 'FECHADO':
            return 'Fechado';
            break;
        case 'NAO_INICIADO':
            return 'Não Iniciado';
            break;
        default:
            return $vigencia;
            break;
    }
}

function postgres_to_php_array($postgresArray)
{
    $postgresStr = trim($postgresArray, '{}');
    $elmts = explode(",", $postgresStr);
    return $elmts;
}

function utf8Decode($string)
{
    return ($string);
}

function sanitizar_string_pesquisa($string, $condicao = false)
{
    return trim(str_to_upper(remove_acentos(($string), $condicao)));
}

function classeUnidadeMedidaCheckBox($arrayPost)
{
    ver($arrayPost);
    return <<<HTML
<input type="checkbox" id="{$arrayPost['codigo']}"/>
HTML;
}

function validarInteiro($valor)
{
    return preg_match('/^-?[0-9]+$/', (string)$valor);
}

function concatenaCpfNome($data, $linha)
{
    return formatar_cpf($data) . ' - ' . $linha['usunome'];
}

function linkArquivo($data, $linha)
{
    $retono = "<a href='?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=documentosApoio&req=download&arqid={$linha['arqid']}'>";
    $retono .= $data;
    $retono .= "</a>";
    return $retono;
}

function formata_status_inativo($status, $linha)
{
    switch ($status) {
        case 'A':
            return '<span style="color: #0D8845; font-weight:bold; ">Ativo</span>';
            break;
        case 'I':
            return 'Inativo <br>' . formata_data_hora($linha['hoddtcriacaoinativacao']);
        ;
            break;
        default:
            return $status;
            break;
    }
}


function linkArquivoAssinaturaCoordenador($data, $linha)
{
    $retono = "<a href='?modulo=sistema/tabelaapoio/tabelasapoio&acao=A&menu=coordenador&requisicao=download&arqid={$linha['arqid']}'>";
    $retono .= $data;
    $retono .= "</a>";
    return $retono;
}

function linkArquivoDocumentoObra($data)
{
    if ($data) {
        $retono = "<a onclick=\"window.location.href = window.location.href+'&req=download&arqid={$data}'\" >";
        $retono .= "<span class=\"btn btn-default btn-sm glyphicon glyphicon-download-alt\"></span>";
        $retono .= "</a>";
    }
    return $retono;
}

function formatarDocumentoObraAnexo($data, $linha)
{
    $arData  = (array) json_decode($data);
    $arArqid = (array) json_decode($linha['arqid']);

    $data = '<div style="text-align:left;">';
    foreach ($arData as $k => $nomeArquivo) {
        $data .= "<a onclick=\"window.location.href = window.location.href + '&requisicao=download&arqid={$arArqid[$k]}'\">
					<span class=\"btn btn-xs glyphicon glyphicon-download-alt\"></span>"
                    . $nomeArquivo .
                 "</a><br>";
    }
    $data .= '</div>';

    return $data;
}

function formatarDocumentoNotaFNDE($data, $linha)
{
    $arData  = (array) json_decode(($data));
    $arArqid = (array) json_decode(($linha['arqid_nota']));
    $data = '';

    if (trim($arData[0])) {
        $data = '<div style="text-align:left;">';
        foreach ($arData as $k => $nomeArquivo) {
            $data .= "<a onclick=\"window.location.href = window.location.href + '&requisicao=download&arqid={$arArqid[$k]}'\">
						<span class=\"btn btn-xs glyphicon glyphicon-download-alt\"></span>"
                        . $nomeArquivo .
                     "</a><br>";
        }
        $data .= '</div>';
    }

    return $data;
}

function linkArquivoDocumentoObraAnexados($data, $linha)
{
    require_once APPRAIZ . '/includes/workflow.php';

    $mdTipoDoc = new Par3_Model_ObraTipoDocumento($data);
    $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
    $mObra     = new Par3_Model_Obra($_GET['obrid']);
    $esd       = wf_pegarEstadoAtual($mObra->docid);
    $pflcod    = pegaPerfilGeral($_SESSION['usucpf']);
    $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
    $naoExcluir = false;

    if (in_array($esd['esdid'], $mAnalise->estadosCorrecaoAnalise()) && !array_intersect($pflcod, $mAnalise->perfisSuperUsuario())) {
        $aeg = $mAnalise->pegarUltimaAnalise($mObra->obrid);
        $aegid = $aeg['aegid'];

        global $db;
        $pendencia = $db->recuperar("
            SELECT * FROM par3.analise_engenharia_obra_documentos_respostas
            WHERE   aegid = {$aegid}
            AND     otdid = {$linha['otdid']}");
        if ((!$pendencia['aed_pendencia'] || $pendencia['aed_pendencia'] != 'S') && !$pendencia['aer_corrigido']) {
            $naoExcluir = true;
        }
    }

    $arqid = json_decode($linha['arqid']);
    $arquivo = json_decode($linha['arquivo']);
    $arqdata = json_decode($linha['arqdata']);
    $odoid = json_decode($linha['odoid']);
    $retono = '';
    if (is_array($arqid) && $arqid[0] != null) {
        foreach ($arqid as $chave => $valor) {
            if (!$_GET['disabled'] && !$naoExcluir) {
                $retono .= "<div style='text-align: left;' id='linhaarquivo" . $odoid[$chave] . "'><a class='btn btn-danger' onclick='removerAnexo(" . $odoid[$chave] . ")'><i class='fa fa-trash'></i></a> ";
            }
            $informacaoArq = $arquivo[$chave]."<br>";
//            $infoDataCriacao = "data-toggle=\"popover\" data-html=\"true\" title=\"Informações\" data-trigger=\"hover\" data-content='Criado em: ".formata_data_hora($arqdata[$chave])."' data-placement=\"auto left\"";
            $link = (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]";
            $retono  .= "<a class='btn btn-default' href='".$link."&req=download&arqid={$arqid[$chave]}'>";
            $retono  .= "<i class='fa fa-download'></i>";
            $retono  .= "</a>";
            $retono  .= "<small class='text-left' > <p>{$informacaoArq} (".formata_data_hora($arqdata[$chave]).")</p></small>";
            $retorno .= "<hr>";
            $retorno.= "</div>";
        }
    }
    return $retono;
}

function linkArquivoDocumentoObraAjuda($data, $linha)
{
    $retono = "<i style='cursor: pointer;' id='otd_{$linha['otdid']}' class=\"glyphicon glyphicon-book button\" onclick='modalAjuda({$data},{$linha['obrid']})'></i>";
    return $retono;
}

function linkAnaliseTipoDocumento($otpid, $linha)
{

    $retono = "

            <i style='cursor: pointer;' id='otd_{$linha['otdid']}' class=\"fa fa-clipboard btn btn-lg\" onclick='modalAnaliseDocumento({$otpid},{$linha['otdid']})'></i>
            ";

//    $retono .= "
//             <script >
//                $(document).find('#otp_{$otpid}_otd_{$linha['otdid']}').parent('td').parent('tr').addClass('danger');
//            </script>";
    return $retono;
}

function finalizarAnalises($obrid)
{
    global $db;
    $db->executar("UPDATE par3.analise_engenharia_obra SET aeg_finalizado = 't' WHERE obrid = {$obrid} ;");
    $db->commit();
    $obr = $db->pegaLinha("select docid from par3.obra where obrid = {$obrid};");
    $hst = $db->pegaLinha("select hstid from workflow.historicodocumento where docid = {$obr['docid']} order by 1 DESC limit 1;");
    //adicionar hstid para o última análise realizada
    $sql = "UPDATE par3.analise_engenharia_obra set
            hstid = {$hst['hstid']}
            WHERE obrid = {$obrid}
            AND aegid = (SELECT aegid from par3.analise_engenharia_obra order by 1 desc limit 1);";
    $db->executar($sql);
    $db->commit();
    return true;
}

function linkAnexarDocumentoObra($data, $linha)
{
    require_once APPRAIZ . '/includes/workflow.php';

    $mdTipoDoc = new Par3_Model_ObraTipoDocumento($data);
    $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();
    $mObra     = new Par3_Model_Obra($_GET['obrid']);
    $esd       = wf_pegarEstadoAtual($mObra->docid);
    $pflcod    = pegaPerfilGeral($_SESSION['usucpf']);
    $mAnalise  = new Par3_Model_AnaliseEngenhariaObra();

    if (in_array($esd['esdid'], $mAnalise->estadosCorrecaoAnalise()) && !array_intersect($pflcod, $mAnalise->perfisSuperUsuario())) {
        $aeg = $mAnalise->pegarUltimaAnalise($mObra->obrid);
        $aegid = $aeg['aegid'];

        global $db;
        $pendencia = $db->recuperar("
            SELECT * FROM par3.analise_engenharia_obra_documentos_respostas
            WHERE   aegid = {$aegid}
            AND     otdid = {$linha['otdid']}");
        if ((!$pendencia['aed_pendencia'] || $pendencia['aed_pendencia'] != 'S') && !$pendencia['aer_corrigido']) {
            return '';
        }
    }

    $mdTipoDoc = new Par3_Model_ObraTipoDocumento($data);

    $mdObraDoc = new Par3_Model_ObraDocumentos();
    $arrDocsObra = $mdObraDoc->recuperarTodos('*', array("otdid = $data", "obrid = {$linha['obrid']}", "odostatus = 'A'"));

    $countArquivos = count($arrDocsObra);
    $maximo = $mdTipoDoc->otdqtd_maximo;
    if ($countArquivos >= $maximo) {
        $retorno = "<a class='btn btn-success btn-large' onclick='msgQuantidadeMaxima()'>";
        $retorno .= 'Anexar Documento';
        $retorno .= "</a>";
    } else {
        $retorno = "<a class='btn btn-success btn-large' onclick='anexarDocumentoObra({$data},{$linha['obrid']})'>";
        $retorno .= 'Anexar Documento';
        $retorno .= "</a>";
    }

        return $retorno;
}


function prepararArraySelectMultiSelecao($arrDados)
{
    $arrFinal = array();
    if (is_array($arrDados)) {
        foreach ($arrDados as $arr) {
            if ($arr == '') {
                continue;
            }
            $arrFinal[] = $arr;
        }
    }
    return $arrFinal;
}

function par3_mascaraMoeda($valor, $align = true, $apenasFormata = false)
{
    if ($valor == null) {
        $valor = 0;
    }

    if (is_numeric($valor)) {
        $valor = number_format($valor, 2, ',', '.');
        if ($apenasFormata) {
            $valor = "R$ " . $valor;
            return $valor;
        }
        if (false !== strpos($valor, '-')) {
            $valor = '<span style="color:red"><b>' . $valor . '</b></span>';
        }
        if ($align === true) {
            $valor = "<p style=\"text-align:right!important\">{$valor}</p>";
        }

        return $valor;
    }
    return $valor;
}

function mascaraReal($valor)
{
    return "R$ ".number_format($valor, 2, ",", ".");
}

function formataNumeroMonetarioComSimbolo($nm)
{
    return 'R$'.number_format($nm, 2, ',', '.');
}

function formataNumeroMonetarioSemSimbolo($nm)
{
    if (is_numeric($nm)) {
        return number_format($nm, 2, ',', '.');
    }
    return $nm;
}
function formatavalorprocesso($nm)
{
    if (is_numeric($nm)) {
        $valor = number_format($nm, 2, ',', '.');
    }
    $html = '<input type="hidden" name="valor_total_processo" id="valor_total_processo" value="'.$nm.'">'.$valor;
    return $html;
}

function formataValorDivergenteEmpenho($nm)
{
    if ( $nm <> 0 ) {
        return '<span style="color: red;">'.number_format($nm, 2, ',', '.').'</spna>';
    } else {
        return '<span style="color: blue;">'.number_format($nm, 2, ',', '.').'</spna>';
    }
}

/**
 * @author Leo Kenzley <leo.oliveira@castgroup.com>
 * @param $vl
 * @return string
 * @description retorna os desdobramentos na lista de obras separados por vírgula
 */
function listaDesdobramentosListaObras($vl)
{
    $stringToClean = array('{','}','"');
    $newValue = str_replace($stringToClean, '', $vl);
    return $newValue;
}

function iconEscolaListaObras($vl)
{
    if (isset($vl)) {
        return "<i class='fa fa-home' title='{$vl}'></i>";
    } else {
        return ' - ';
    }
}

/**
 * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
 * @param array $array
 * @return string
 * @description esta funçao retorna uma string e limpa os dados nulos a partir de um array, separados por aspas simples e e vírgula
 * exemplo String: array('AC','RJ','XPTO') > string: 'AC','RJ','XPTO' | $this->retornValuesStringINSQL($array,'string'); OU $this->retornValuesStringINSQL($array);
 * exemplo Integer: array(1,2,34) > string: 1,2,34 $this->retornValuesStringINSQL($array,'int')
 */
function retornValuesStringINSQL(array $array, $typeReturn = null)
{
    $stringIN = '';
    if (is_array($array) && $typeReturn ==  'string' || $typeReturn == null) {
        //string ou null
        for ($i = 0; $i<=count($array); $i++) {
            if (!empty($array[$i])) {
                if ($i==0) {
                    if (count($array)>1) {
                        $stringIN .= " '".$array[$i]."', ";
                    } else {
                        $stringIN .= " '".$array[$i]."' ";
                    }
                }
                if ($i > 0 && $array[$i] != '') {
                    if ($i < (count($array)-1)) {
                        $stringIN .= " '".$array[$i]."', ";
                    } else {
                        $stringIN .= " '".$array[$i]."' ";
                    }
                }
            }
        }
    } elseif (is_array($array) && $typeReturn ==  'int') {
        //int
        for ($i = 0; $i<=count($array); $i++) {
            if (!empty($array[$i])) {
                if ($i==0) {
                    if (count($array)>1) {
                        $stringIN .= $array[$i].",";
                    } else {
                        $stringIN .= $array[$i];
                    }
                }
                if ($i > 0 && $array[$i] != '') {
                    if ($i < (count($array)-1)) {
                        $stringIN .= $array[$i].",";
                    } else {
                        $stringIN .= $array[$i];
                    }
                }
            }
        }
    }
    return $stringIN;
}
function formataStatusSwuitchery($var1, $var2, $var3)
{
    $status = $var1;
    $id = $var3;

    switch ($status) {
        case 'A':
            return '<input type="checkbox" class="js-switch" id="'.$id.'" value="'.$status.'" onchange="alterasituacao(this.id,this.value);" checked />';
            break;
        case 'I':
            return '<input type="checkbox" class="js-switch" id="'.$id.'" value="'.$status.'" onchange="alterasituacao(this.id,this.value);" />';
            break;
        default:
            return $status;
            break;
    }
}

function genericReplaceArrayAgg($v1)
{
    if ($v1 != '') {
        $newV1 = str_replace(array("{","}"), '', $v1);
        return $newV1;
    }
    return '';
}

function enviaEmailDiligenciaIniciativa($inpid, $anaid)
{
    global $db;
    //ver($inpid, $inaid,d);

    $sql = "SELECT
				iu.itrid, iu.estuf, iu.muncod
			FROM
				par3.instrumentounidade iu
                inner join par3.iniciativa_planejamento ip on ip.inuid = iu.inuid
            WHERE ip.inpid = ".$inpid;

    $dadosEntidade = $db->pegaLinha($sql);

    $anaano = $db->pegaUm("select anaano from par3.analise where anaid = $anaid");
    $iniciativa = $db->pegaUm("select ini.iniid||' - '||id.inddsc from par3.iniciativa_planejamento ip
								inner join par3.iniciativa ini on ini.iniid = ip.iniid
							    inner join par3.iniciativa_descricao id on id.indid = ini.indid
							where ip.inpid = $inpid");

    if ($dadosEntidade['itrid'] == 1) { //Estado
        $sql = "SELECT
					usunome,
					usuemail
				FROM
					par3.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = usuario.usucpf AND us.sisid = 231 AND us.susstatus = 'A' AND us.suscod = 'A'
				WHERE
					ur.pflcod IN (".PAR3_PERFIL_EQUIPE_ESTADUAL.") AND
					ur.rpustatus = 'A' AND
					ur.estuf = '".$dadosEntidade['estuf']."'";
    } else { //Município
        $sql = "SELECT
					usunome,
					usuemail
				FROM
					par3.usuarioresponsabilidade ur
				INNER JOIN seguranca.usuario usuario ON usuario.usucpf = ur.usucpf
				INNER JOIN seguranca.usuario_sistema us ON us.usucpf = usuario.usucpf AND us.sisid = 231 AND us.susstatus = 'A' AND us.suscod = 'A'
				WHERE
					ur.pflcod IN (".PAR3_PERFIL_PREFEITO.", ".PAR3_PERFIL_EQUIPE_MUNICIPAL.") AND
					ur.rpustatus = 'A' AND
					ur.muncod = '".$dadosEntidade['muncod']."'";
    }

    $arDadosUsuarios = $db->carregar($sql);
    $arDadosUsuarios = $arDadosUsuarios ? $arDadosUsuarios : array();

    $cc  = "";
    $cco = "";
    //  $cc = "planodemetas@mec.gov.br";
    //  $cco = "victor.benzi@mec.gov.br";

    $assunto  = "MEC/FNDE - PAR com iniciativa(as) em diligência - ".$iniciativa.'/'.$anaano;

    $conteudo = '<p>Prezado(a) Senhor(a),</p>
	
				<p>O Plano de Ações Articuladas (PAR 2016/2019) possui iniciativa(s) na situação "em diligência".</p>
	
				<p>Por favor, acesse o seu planejamento no PAR(2016/2019), filtre as iniciativa(s) em diligência no campo situação, solucione a(s) pendência(s) indicada(s) no Parecer e encaminhe novamente a(s) iniciativa(s) para análise do MEC.</p>
	
				<p>Atenciosamente,</p>
	
	
				Equipe Técnica do Plano de Ações Articuladas - PAR<br/>
				Fundo Nacional de Desenvolvimento da Educação<br/>
				Ministério da Educação<br/>
				Telefones: (61) 2022-5854 / 5382/ 5953/ 5815.<br/>
				PAR Fale Conosco. Link: https://www.fnde.gov.br/parfaleconosco/index.php/publico (Acesso site FNDE), ou pelo Fale Conosco disponível na aba inferior do módulo PAR (Acesso site SIMEC).';

    if ($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao") {
        enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), 'wesleysilva@mec.gov.br', $assunto, $conteudo, $cc, $cco);
    } else {
        foreach ($arDadosUsuarios as $dados) {
            $remetente = array('nome'=>$dados['usunome'], 'email'=>'simec@mec.gov.br');
            enviar_email(array('nome'=>'SIMEC - PAR', 'email'=>'noreply@mec.gov.br'), $dados['usuemail'], $assunto, $conteudo, $cc, $cco);
        }
    }
    return true;
}

function trataEnvioAnalise($inpid, $anaid)
{
    global $db;

    $docid = $db->pegaUm("select docid from par3.iniciativa_planejamento where inpid = {$inpid}");
    $esdid = $db->pegaUm("select esdid from workflow.documento where docid = {$docid}");

    if ($esdid == PAR3_ESDID_EM_DILIGENCIA || $esdid == PAR3_ESDID_EM_ANALISE) {
        return true;
    } else {
        $retorno = wf_alterarEstado($docid, 5165, 'Fluxo de Iniciativas do PAR3', array());
        return $retorno;
    }
}

function trataEnvioDiligencia($inpid, $anaid)
{
    global $db;

    $docid = $db->pegaUm("select docid from par3.iniciativa_planejamento where inpid = {$inpid}");
    $esdid = $db->pegaUm("select esdid from workflow.documento where docid = {$docid}");
    

    if ($esdid == PAR3_ESDID_EM_DILIGENCIA) {
        $sql = "SELECT anaano FROM par3.analise WHERE anaid = {$anaid}";

        $iniano = $db->pegaUm($sql);
        
        salvarHistoricoAnalise($inpid, $anaid, $iniano);
        return true;
    } else {
        $sql = "select ed.esdid, ed.esddsc from workflow.documento d
				inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where docid = $docid";
        $arEstato = $db->pegaLinha($sql);

        $sql = "select
						a.aedid,
						a.aeddscrealizar,
						a.aeddscrealizada
					from workflow.acaoestadodoc a
						inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
					where
						esdidorigem = {$arEstato['esdid']} and
						esdiddestino = ".PAR3_ESDID_EM_DILIGENCIA." and
						aedstatus = 'A' and
						aedvisivel = true
					order by a.aedordem asc";
        $arAcao = $db->pegaLinha($sql);
        $retorno = wf_alterarEstado($docid, $arAcao['aedid'], 'Fluxo de Iniciativas do PAR3', array());

        if ($retorno) {
            enviaEmailDiligenciaIniciativa($inpid, $anaid);
        }

        return $retorno;
    }
}

function trataEnvioDiligenciaIniciativaAnalise($inpid)
{
    global $db;

    $docid = $db->pegaUm("select docid from par3.iniciativa_planejamento where inpid = {$inpid}");

    $sql = "select d.docid, a.anaano, a.anaid from par3.analise a
			inner join workflow.documento d on d.docid = a.docid
			where a.inpid = {$inpid}
				and d.esdid = ".PAR3_ESDID_ANALISE_DILIGENCIA;
    $arAnalise = $db->carregar($sql);
    $arAnalise = $arAnalise ? $arAnalise : array();

    foreach ($arAnalise as $v) {
        $sql = "select ed.esdid, ed.esddsc from workflow.documento d
				inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where docid = {$v['docid']}";
        $arEstato = $db->pegaLinha($sql);

        $sql = "select
						a.aedid,
						a.aeddscrealizar,
						a.aeddscrealizada
					from workflow.acaoestadodoc a
						inner join workflow.estadodocumento ed on ed.esdid = a.esdiddestino
					where
						esdidorigem = {$arEstato['esdid']} and
						esdiddestino = ".PAR3_ESDID_PLANEJAMENTO_AGUARDANDO_ANALISE." and
						aedstatus = 'A'
					order by a.aedordem asc";

        $arAcao = $db->pegaLinha($sql);

        $retorno = wf_alterarEstado($v['docid'], $arAcao['aedid'], 'Fluxo de Iniciativas do PAR3', array('inpid' => $inpid, 'anaid' => $v['anaid']));
    }
    return $retorno;
}

function validaEnvioValidacaoCoordenador($inpid, $anaid, $iniano = 0)
{
    global $db;

    $retorno = false;
    // Verifica se existe pelo menos um iten de composicao
    if ($iniano != 0) {
        $dados['inpid'] = $inpid;
        $dados['iniano'] = $iniano;
        $dados['anaid'] = $anaid;

        $totalItens = verificaTotalItensAprovadosAnalisePlanejamento($dados);
        if ($totalItens <= 0) {
            return "Não existem itens de composição com quantidade aprovada";
        }
    } else {
        return "Erro ao recuperar os dados dos itens de composição";
    }

    if ($anaid) {
        $sql = "select a.anaparecer from par3.analise a where a.anaid = $anaid";
        $anaparecer = $db->pegaUm($sql);

        if (empty($anaparecer)) {
            $retorno = 'Informe a situação do parecer';
        } elseif ($anaparecer == 'A') {
            return true;
        } elseif ($anaparecer == 'D') {
            $retorno = 'Situação do parecer em diligência';
        } elseif ($anaparecer == 'R') {
            $retorno = 'Situação do parecer reprovado';
        }
    }

    return $retorno;
}

function verificaTotalItensAprovadosAnalisePlanejamento($dados)
{
    global $db;
    // Recupero os parâmetros para buscar os iigid
    $inpid = $dados['inpid'];
    $iniano = $dados['iniano'];
    $anaid = $dados['anaid'];

    $sql = "select iigid from par3.iniciativa_planejamento_item_composicao where inpid = {$inpid} and ipiano = $iniano and ipistatus = 'A' group by iigid;";
    $itensCadastrados = $db->carregarColuna($sql);
    // Caso hajam itens cadastrados, senão retorna 0 pois não existem itens ainda
    if ($itensCadastrados) {
        // Carrega os iigids
        $iigid = implode(', ', $itensCadastrados);
        // caso existam itens cadastrados e for um array
        if (is_array($itensCadastrados)) {
            // busca os dados
            $sql = "select distinct iic.iigid, iig.itcid, iig.igrid from par3.iniciativa_planejamento_item_composicao  iic
			inner join par3.iniciativa_itenscomposicao_grupo iig on iig.iigid = iic.iigid --and iig.iigsituacao = 'A'
			where iic.inpid = {$inpid} and iic.iigid in ({$iigid}) and iic.ipistatus = 'A'";
            // PEga apenas uma linha (segui o que está na função que lista os itens na tela)
            $iicg = $db->pegaLinha($sql);

            $sqlTotItem = '';
            // Verifica os tipos dos itens
            if ($iicg['itcid']) {
                // Busca itens que estejam aprovados e com quantidade aprovada maior que 0
                $sqlTotItem = "
				SELECT  count(distinct ipc.ipiid)
		                    FROM par3.iniciativa_planejamento ip
		                    JOIN par3.iniciativa i USING (iniid)
		                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
		                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
		                    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
		                    JOIN par3.itenscomposicao ic USING (itcid)
		                    JOIN par3.unidade_medida um USING (uniid)
		                WHERE
					ip.inpid = {$inpid} and iia.iniano = '{$iniano}' and ipc.anaid = $anaid
				AND
					ipc.ipiaprovado in( 'S', 'CS')
				AND
					ipiquantidadeaprovada > 0
				AND
					iia.iianostatus = 'A'
				
				";
            } elseif ($iicg['igrid']) {
                // Busca itens que estejam aprovados e com quantidade aprovada maior que 0
                $sqlTotItem = "SELECT  count(distinct ipc.ipiid)
                    FROM par3.iniciativa_planejamento ip
                    JOIN par3.iniciativa i USING (iniid)
                    JOIN par3.iniciativa_iniciativas_anos iia USING (iniid)
                    JOIN par3.iniciativa_itenscomposicao_grupo iicg USING (iniid)
                    JOIN par3.itenscomposicao_grupos icg USING (igrid)
                    JOIN par3.itenscomposicao_grupos_itens icgi USING (igrid)
                    inner join par3.v_analise_planejamento_item_composicao ipc on ipc.inpid = ip.inpid and ipc.iigid = iicg.iigid and ipc.ipistatus = 'A' and ipc.ipiano = iia.iniano
                    WHERE
						ip.inpid =  {$inpid} and iia.iniano = '{$iniano}' and ipc.anaid = $anaid
					AND
						iia.iianostatus = 'A'
					AND
						ipc.ipiaprovado in( 'S', 'CS')
					AND
						ipiquantidadeaprovada > 0";
            }
            // Caso não traga nenhuma informação para buscar os itens retorna erro
            if ($sqlTotItem == '') {
                return 0;
            } else {
                // Verifica se existem os itens validados e com quantidade aprovada, caso não possua retorna que não possui
                $totalItens = $db->pegaUm($sqlTotItem);
                if (! $totalItens) {
                    return 0;
                }
                return $totalItens;
            }
        }
    } else {
        return 0;
    }
}

function validaEnvioAnaliseDiligencia($inpid, $anaid)
{
    global $db;

    if ($anaid) {
        $sql = "select a.anaparecer from par3.analise a where a.anaid = $anaid";
        $anaparecer = $db->pegaUm($sql);

        if (empty($anaparecer)) {
            echo 'Informe a situação do parecer';
        } elseif ($anaparecer == 'A') {
            $retorno = 'Situação do parecer Aprovado';
        } elseif ($anaparecer == 'D') {
            return true;
        } elseif ($anaparecer == 'R') {
            $retorno = 'Situação do parecer reprovado';
        }
    }

    return false;
}

function validaEnvioAnaliseReprovado($inpid, $anaid)
{
    global $db;

    if ($anaid) {
        $sql = "select a.anaparecer from par3.analise a where a.anaid = $anaid";
        $anaparecer = $db->pegaUm($sql);

        if (empty($anaparecer)) {
            echo 'Informe a situação do parecer';
        } elseif ($anaparecer == 'A') {
            $retorno = 'Situação do parecer Aprovado';
        } elseif ($anaparecer == 'D') {
            $retorno = 'Situação do parecer em diligência';
        } elseif ($anaparecer == 'R') {
            return true;
        }
    }

    return false;
}

function trataEnvioPlanejamentoAprovado($anaid)
{
    global $db;

    $sql = "UPDATE par3.analise SET cpfcoordenador = '".$_SESSION['usucpf']."'
			 WHERE anaid = $anaid";

    $db->executar($sql);
    return $db->commit();
}

//
//function formataSituacaoObraEstadoMunicipio($vl,$vl2){
//   $arraySituacaoDiferenteDe = array(2007,2054,2052,2053,2050);
//   $arraySituacaoIgualA = array(2044 => 'Aguardando correção pelo Município/Estado',2056 =>"Em reformulação / aguardando atualização da obra pelo Município/Estado" ,2058 => "Em reformulação / aguardando correção pelo Município/Estado");
//   if(in_array($vl,$arraySituacaoIgualA) && !in_array($vl,$arraySituacaoDiferenteDe)){
//        return $arraySituacaoIgualA[$vl];
//   }elseif(!in_array($vl,$arraySituacaoIgualA) && in_array($vl,$arraySituacaoDiferenteDe)){
//       if(!in_array($vl,$arraySituacaoDiferenteDe)){
//            return "Aguardando Análise do FNDE";
//       }
//   }else{
//       return $vl2['situacao_estado_documento'];
//   }
//
//}

function formataSituacaoObraEstadoMunicipio($vl, $vl2)
{


    $arraySituacaoDiferenteDe = array(2007,2054,2052,2053,2050,2061);
    $arraySituacaoIgualA = array(2044 => 'Aguardando correção pelo Município/Estado',2056 =>"Em reformulação / aguardando atualização da obra pelo Município/Estado" ,2058 => "Em reformulação / aguardando correção pelo Município/Estado");



    if (array_key_exists($vl, $arraySituacaoIgualA) && !in_array($vl, $arraySituacaoDiferenteDe)) { // ta na igual e não está na diferente
        return $arraySituacaoIgualA[$vl];
    } elseif (!array_key_exists($vl, $arraySituacaoIgualA) && !in_array($vl, $arraySituacaoDiferenteDe)) { //não está na igual e não está  na diferente
        return "Aguardando Análise do FNDE";
    } else {
        return $vl2['situacao_estado_documento'];
    }
}

function consultaMunicipios($uf)
{
    $municipio = new Territorios_Model_Municipio();
    return $municipio->lista(array('muncod', 'mundescricao'), array("estuf = '$uf'"), null, array('order'=>'mundescricao'));
}

// Chaveador de serviços pontuais compartilhados do módulo.
function verificaServicos()
{
    switch ($_REQUEST['servico']) {
        case 'municipios':
            print simec_json_encode(consultaMunicipios($_REQUEST['uf']));
            die;
    }
}

function trata_salvar($arrParam = array())
{
    global $db;

    $chave = $arrParam['arChavePrimaria']; //$this->model->arChavePrimaria[0];
    $tabela = $arrParam['stNomeTabela']; //$this->model->stNomeTabela;
    $arAtributos = $arrParam['arAtributosTabela']; //$this->model->arAtributosTabela;

    $action = 'insert';
    if (!empty($arrParam[$chave])) {
        $action = 'update';
    }

    $codigo = '';
    if ($action == 'insert') {
        $arCampos = array();
        $arValor = array();
        foreach ($arrParam as $campo => $valor) {
            if ($valor !== null && $valor !== '' && in_array($campo, $arAtributos)) {
                $arCampos[]  = $campo;
                $valor = str_replace($troca, "", $valor);
                $arValor[] = trim(pg_escape_string($valor));
            }
        }

        if ($chave) {
            $sql = "insert into ".$tabela." (".implode(', ', $arCampos) ." ) values( '". implode("', '", $arValor)."') returning ".$chave.";";
            $codigo = $db->pegaUm($sql);
        } else {
            $sql = "insert into ".$tabela." (".implode(', ', $arCampos) ." ) values( '". implode("', '", $arValor)."');";
            $db->executar($sql);
        }
    } else {
        $campos = "";
        foreach ($arrParam as $campo => $valor) {
            if ((!empty($valor) || is_bool($valor) || ($valor == 0)) && in_array($campo, $arAtributos)) {
                if ($campo == $chave) {
                    $codigo = $valor;
                } elseif (is_bool($valor)) {
                    $campos .= $campo." = ".($valor ? 'true' : 'false').", ";
                } else {
                    $valor = pg_escape_string($valor);
                    if( $valor == 'null' ) $campos .= $campo." = ".$valor.", ";
                    else $campos .= $campo." = '".$valor."', ";
                }
            }
        }
        $campos = substr($campos, 0, -2);

        $sql = " UPDATE ".$tabela." SET $campos WHERE ".$chave." = $codigo;";
        $db->executar($sql);
    }
    insereArquivo($sql);
    $db->commit();
    return $codigo;
}

function insereArquivo( $sql ){
    if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
        $nomeArquivo 		= 'DocumentoTermo_'.date('Ymd');
        $diretorio		 	= APPRAIZ . 'arquivos/par3';
        $diretorioArquivo 	= APPRAIZ . 'arquivos/par3/'.$nomeArquivo.'.txt';
        
        if( !is_dir($diretorio) ){
            mkdir($diretorio, 0777);
        }
        
        $fp = fopen($diretorioArquivo, "a+");
        if ($fp) {
            stream_set_write_buffer($fp, 0);
            fwrite($fp, $sql);
            fclose($fp);
        }
    }
}

function trataEnvioValidacaoCoordenador($inpid, $anaid, $iniano)
{
    global $db;
    $sql = "SELECT inpid, ipiano, sum(vlr_aprovado) AS vlr_aprovado, sum(vlr_emenda) AS vlr_emenda FROM(
            	SELECT va.inpid, va.ipiano, va.ipiid, (ipiquantidadeaprovada * ipivalorreferencia) AS vlr_aprovado, (em.quantidade * ipivalorreferencia) AS vlr_emenda
            	FROM par3.v_analise_planejamento_item_composicao va
            		LEFT JOIN(
            			SELECT ie.inpid, ie.edeid, ic.ipiid, sum(ic.ieiquantidade) AS quantidade FROM par3.iniciativa_emenda ie
            				INNER JOIN par3.iniciativa_emenda_item_composicao ic ON ic.ineid = ie.ineid
            			WHERE ieistatus = 'A'
            				AND ic.ieistatus = 'A'
            			GROUP BY ie.inpid, ie.edeid, ic.ipiid
            		) em ON em.inpid = va.inpid AND em.edeid = va.edeid AND em.ipiid = va.ipiid
            	WHERE anaid = $anaid
            		AND ipiquantidadeaprovada > 0
            ) AS foo
            GROUP BY inpid, ipiano";
    $arDados = $db->pegaLinha($sql);
    
    $boEmenda = $db->pegaUm("SELECT count(anaid) FROM par3.analise WHERE intaid = 2 AND anastatus = 'A' AND anaid = $anaid");
    
    $contrapartida = 0;
    if( $boEmenda > 0 ){
        if ((float)$arDados['vlr_aprovado'] > (float)$arDados['vlr_emenda']) {
            $contrapartida = ((float)$arDados['vlr_aprovado'] - (float)$arDados['vlr_emenda']);
        }
    }
    $db->executar("UPDATE par3.analise SET anacontrapartida = $contrapartida WHERE anaid = $anaid");
    $db->commit();
    
   salvarHistoricoAnalise($inpid, $anaid, $iniano);
   
   return true;
}
/**
 * @param $vl
 * @param $inpid
 * @param $anaid
 * @param $iniano
 * @description Salva o histórico do parece a cada saída do estado "Em análise"
 */
function salvarHistoricoAnalise($inpid, $anaid, $iniano)
{
    global $db;
    
    
    if(!$_REQUEST['docid']){
        $docid = $db->pegaUm("select docid from par3.iniciativa_planejamento where inpid = {$inpid}");
    }else{
        $docid = $_REQUEST['docid'];
    }

    $sql = "SELECT * FROM par3.analise WHERE docid = {$docid};";
    $arDados = $db->pegaLinha($sql);

    if (empty($arDados)) {
        return false;
    } else {
        extract($arDados);
        $sql = "INSERT INTO
              par3.analise_historico( anaid, inpid, anaano, docid, anaconsideracaoent, anaconsideracaoprop, anaconsideracaoproj, anaconsideracaoobj, anaconsideracaojus, anaconsideracaoval,
              anaoutrasconsideracoes, anaparecer, anatextoparecer, cpftecnico, anadatacriacao, anastatus, usucpfcriacao )
              VALUES (
                  $anaid,
                  $inpid,
                  $iniano,
                  $docid,
                  '{$anaconsideracaoent}',
                  '{$anaconsideracaoprop}',
                  '{$anaconsideracaoproj}',
                  '{$anaconsideracaoobj}',
                  '{$anaconsideracaojus}',
                  '{$anaconsideracaoval}',
                  '{$anaoutrasconsideracoes}',
                  '{$anaparecer}',
                  '{$anatextoparecer}',
                  '{$_SESSION['usucpf']}',
                  'now()',
                  'A',
                  '{$_SESSION['usucpf']}'
            ) RETURNING anhid;";
        $retorno = $db->pegaUm($sql);
        return $retorno ? true : false;
    }
}

//modal de contratos em execução e acompanhamento
function campoButtonIcon($id)
{
    return <<<HTML
<button class=" visualizar_item btn btn-success btn-sm" type="button" onclick="modalContratosItem({$id})"><i class="fa fa-search"></i></button>
HTML;
}

function retiraPontos($valor)
{
    $valor = str_replace(".", "", $valor);
    $valor = str_replace(",", ".", $valor);

    return $valor;
}

function existeAnalise($edlid)
{
    global $db;

    $sql = "SELECT eadid FROM par3.execucao_analise_documentolicitacao WHERE edlid = {$edlid} AND eadstatus = 'A'";
    $id = $db->pegaUm($sql);
    if ($id > 0) {
        return true;
    }
    return 'É necessário salvar um parecer!';
}

function getToobarListagem($sql, $boPesquisa = true)
{
    global $db;

    $html = '
    <script type="text/javascript">
jQuery.getScript(\'/library/simec/js/listagem.js\');
jQuery(function(){
    jQuery(document).on(\'click\', \'.navbar-listagem .btn-query\', function(){
        jQuery(\'#listagem-query.modal .modal-body\').empty().html(
            \'<pre>\' + jQuery(this).attr(\'data-query\') + \'</pre>\'
        );
        jQuery(\'#listagem-query\').modal();
    });
    $(\'[data-toggle="popover"]\').popover();
    $(\'[data-toggle="popover"]\').mouseleave(function(e) {
        e.stopPropagation();
        $(document).find(\'.popover\').remove();
    });
});
</script>
    <div class="modal fade" id="listagem-query" style="z-index:99999">
  <div class="modal-dialog modal-lg">
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Visualização de query</h4>
      </div>
      <div class="modal-body">
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-success" data-dismiss="modal">Ok</button>
      </div>
    </div>
  </div>
</div>
    <input type="hidden" name="listagem[p]" class="listagem-p" value="1" />
        <input type="hidden" name="listagem[campo]" id="listagem-campo-ordenacao" value="" />
        <input type="hidden" name="listagem[sentido]" id="listagem-campo-sentido" value="" />
        <nav class="navbar navbar-default navbar-listagem tb_class_render" role="navigation" style="z-index: 0">
            <div class="navbar-form navbar-left btn-group">
                <button type="button" class="btn btn-default btn-xls"
                    data-toggle="popover"
                    data-trigger="hover"
                    data-placement="auto top"
                    data-title=\'Ajuda - Exportar XLS\'
                    data-content="Exporta o conteúdo do relatório para o formato XLS (Excel)." data-id-lista="tb_render">
                    <span class="glyphicon glyphicon-download-alt"></span>
                </button>
                <button type="button" class="btn btn-default btn-pdf"
                data-toggle="popover"
                data-trigger="hover"
                data-placement="auto top"
                data-title=\'Ajuda - Exportar PDF\'
                data-content="Exporta o conteúdo do relatório para o formato PDF (Adobe Reader)."
                data-id-lista="tb_render">
                    <span class="glyphicon glyphicon-file"></span>
                </button>
                <button type="button" class="btn btn btn-default btn-query"
                data-title=\'Ajuda - Query\'
                                data-toggle="popover"
                data-trigger="hover"
                data-placement="auto top"
                data-content="Exibe a query utilizada na listagem." data-query="'.$sql.'">
                    <span class="glyphicon glyphicon-cog"></span>
                </button>
            </div>';
    if ($boPesquisa) {
        $html .= '  <div class="navbar-form navbar-left" style="text-align:right;font-weight:bold">
                <span>Pesquisa<br />rápida:</span>
            </div>
            <div class="navbar-form navbar-pesquisa">
                <div class="input-group">
                    <div class="input-group-addon"
                    data-toggle="popover"
                    data-trigger="hover"
                    data-placement="auto top"
                    data-toggle="popover"
                    style="cursor:pointer">
                        <span class="glyphicon glyphicon-info-sign" style="color:#428bca"></span>
                    </div>
                    <input class="form-control busca-listagem" type="text" id="textFind" placeholder="Digite o texto para busca" />
                </div>
            </div>';
    }
    $html .= '</nav>';
    $arrPerfil = pegaPerfils($_SESSION['usucpf']);
    if( in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, $arrPerfil) ){
        return $html;
    } else {
        return '';   
    }
}

function number_format_par3($number)
{
    if (!is_numeric($number)) {
        return '0,00';
    }
    $decimals = is_numeric($decimals) ? $decimals : 0;
    if (substr($number, strpos($number, '.')+1) > 0) {
        return number_format($number, 2, ',', '.');
    } else {
        return number_format($number);
    }
}

function tituloSaldoProcessoTabela($campo)
{
    return "<span title='Composto por Conta Corrente + Conta Poupança + Fundo' style='cursor: pointer'>R$".number_format($campo, 2, ',', '.')."</span>";
}

function posAcaoAtualizarValorSaldo($sdpid, $valorSolicitado, $saldoTermo)
{
    global $db;
    $saldo = bcsub($saldoTermo, $valorSolicitado, 2);
    $sql = "UPDATE par3.solicitacao_desembolso_par SET sdpsaldotermo = {$saldo} WHERE sdpid = {$sdpid}";
    $db->executar($sql);
    return $db->commit();
}

function validaRetornoValidacaoCoordenador($inpid, $anaid)
{
    global $db;
    
    $sql = "SELECT count(pp.proid) FROM par3.processoparcomposicao pp
            WHERE pp.anaid = $anaid AND pp.inpid = $inpid AND pp.ppcstatus = 'A'";
    $bototal = $db->pegaUm($sql);
    
    if ((int)$bototal > (int)0) {
        return 'Está análise já está vinculada a um processo. Para nova análise é necessário realizar a desvinculação na administração de processo!';
    } else {
        return true;
    }
}

function condicaoExisteAnaliseDesembolso($sdpid)
{
    global $db;

    $sql = "SELECT sdpid FROM par3.analise_solicitacao_desembolso WHERE sdpid = {$sdpid} AND asdstatus = 'A'";
    $id = $db->pegaUm($sql);
    if ($id > 0) {
        return true;
    }
    return 'É necessário salvar um parecer!';
}

function formataTipoDocumentoParams($termo,$arCampos)
{
    if($arCampos['dotid']){
        return '<a class="termo_detalhe"><span class="dotid" value='.$arCampos['dotid'].'>'.$termo.'</span></a>';
    }
}

function hintPorcentagemPagamento($porcentagem)
{
    return '<span title="% de pagamento baseado no valor empenhado">' . $porcentagem . '</span>';

}


function radioEventoEmpenho($empid)
{
    if(!$empid) {
        return '';
    }
    $radio = <<<HTML
        <div class="radio radio-success">
        <input type="radio" name="empid[]" id="chkempenho_$empid" value="$empid" onchange="javascript:selecionarEmpenhoPagamento($empid);">
        <label for="chkempenho_$empid" style="margin-left: 10px">&nbsp;</label>
        </div>
HTML;
    return $radio;
}

function formataPendencia($pendencia)
{
    $icone = '';
    switch ($pendencia) {
        case 1: $icone  = 'Sem Pendência';break;
        case 2: $icone  = 'Pendência';break;
        case 3: $icone  = 'Alerta';break;
        default: $icone = $pendencia;
    }
    return $icone;
    return;
}
    // função utilizada para cadastrar os dados dos diretores do PDE para envio de emails via rotina
    function listaDiretorEscolaSelecionada(){
        
        global $db;

        $verificacao = (string) $_REQUEST['verificacao'];
        
        if ('s' == $_REQUEST['bs']) {
            $verificacao = urldecode($verificacao);
        }
        $dados = unserialize( stripcslashes( $verificacao ) );
        //$dados = unserialize($_REQUEST['verificacao']);
        $inuid = $dados['inuid'];
        
        if ($inuid && $_SESSION['exercicio']) {
            $sql = "SELECT
                        aea.codinep
                    FROM
                        par3.adesaoescolaacessivel aea
                    INNER JOIN par3.prodesaoprograma pdp
                        ON aea.adpid = pdp.adpid
                    WHERE
                        aea.aesstatus = 'A'
                        AND pdp.adpano = '{$_SESSION['exercicio']}'
                        AND aea.inuid = {$inuid}
                        AND aea.codinep
                        NOT IN ( SELECT eemacodinep FROM par3.envioemailescolaacessivel WHERE eemastatus = 'E' and eemastatus is not null ) ";

            $res = $db->carregar($sql);

            if ($res) {
                $arrayCodinep = array();

                foreach ($res as $key => $value) {
                    $codinep = (string) $value['codinep'];
                    array_push($arrayCodinep, $codinep);
                }
                $resCodinep = implode("','", $arrayCodinep);

                /*$sqlAdesaoPDE = "SELECT DISTINCT ent.entcodent, ent.entnome, u.usunome, u.usucpf, u.usuemail
                                FROM escolaacessivel.usuarioresponsabilidade ur
                                INNER JOIN entidade.entidade ent ON ent.entid = ur.entid
                                INNER JOIN seguranca.usuario u ON ur.usucpf = u.usucpf
                                WHERE ur.rpustatus = 'A'
                                AND ent.entstatus = 'A'
                                AND ent.entcodent in ('{$resCodinep}') ORDER BY u.usunome ASC; ";
                */
                $sqlAdesaoPDE = "(SELECT DISTINCT ent.entcodent, ent.entnome, u.usunome, u.usucpf, u.usuemail
                                    FROM escolaacessivel.usuarioresponsabilidade ur
                                    INNER JOIN entidade.entidade ent ON ent.entid = ur.entid
                                    INNER JOIN seguranca.usuario u ON ur.usucpf = u.usucpf
                                    WHERE ur.rpustatus = 'A'
                                    --AND ent.entstatus = 'A'
                                    AND ent.entcodent in 
                                    ('{$resCodinep}')
                                    ORDER BY u.usunome ASC)
                                    union 
                                (SELECT DISTINCT ent.entcodent, ent.entnome, u.usunome, u.usucpf, u.usuemail
                                    FROM pdeinterativo.usuarioresponsabilidade ur
                                    INNER JOIN entidade.entidade ent ON ent.entid = ur.entid
                                    INNER JOIN seguranca.usuario u ON ur.usucpf = u.usucpf
                                    WHERE ur.rpustatus = 'A'
                                    --AND ent.entstatus = 'A'
                                    AND ent.entcodent in 
                                    ('{$resCodinep}')
                                    ORDER BY u.usunome ASC
                                )";

                $dadosDiretores = adapterConnection::pddeinterativo()->carregar($sqlAdesaoPDE);

                // insere diretores na tabela de emails para envio na rotina
                if ( is_array($dadosDiretores) && !empty($dadosDiretores) ) {
                    foreach ($dadosDiretores as $key => $e) {
                        $sql = "INSERT INTO par3.envioemailescolaacessivel (eemanomediretor, eemaemaildiretor, eemacodinep, eemanomeescola, eemadtenvio, eemastatus) VALUES ('{$e['usunome']}', '{$e['usuemail']}', '{$e['entcodent']}', '{$e['entnome']}', 'NOW()', 'P')";
                        $db->executar($sql);
                        $db->commit();
                    }
                }
            }
        }
        return true;
    }

if (!function_exists('array_group_by')) {
    /**
     * Groups an array by a given key.
     *
     * Groups an array into arrays by a given key, or set of keys, shared between all array members.
     *
     * Based on {@author Jake Zatecky}'s {@link https://github.com/jakezatecky/array_group_by array_group_by()} function.
     * This variant allows $key to be closures.
     *
     * @param array $array   The array to have grouping performed on.
     * @param mixed $key,... The key to group or split by. Can be a _string_,
     *                       an _integer_, a _float_, or a _callable_.
     *
     *                       If the key is a callback, it must return
     *                       a valid key from the array.
     *
     *                       If the key is _NULL_, the iterated element is skipped.
     *
     *                       ```
     *                       string|int callback ( mixed $item )
     *                       ```
     *
     * @return array|null Returns a multidimensional array or `null` if `$key` is invalid.
     */
    function array_group_by(array $array, $key)
    {
        if (!is_string($key) && !is_int($key) && !is_float($key) && !is_callable($key) ) {
            trigger_error('array_group_by(): The key should be a string, an integer, or a callback', E_USER_ERROR);
            return null;
        }
        $func = (!is_string($key) && is_callable($key) ? $key : null);
        $_key = $key;
        // Load the new array, splitting by the target key
        $grouped = [];
        foreach ($array as $value) {
            $key = null;
            if (is_callable($func)) {
                $key = call_user_func($func, $value);
            } elseif (is_object($value) && isset($value->{$_key})) {
                $key = $value->{$_key};
            } elseif (isset($value[$_key])) {
                $key = $value[$_key];
            }
            if ($key === null) {
                continue;
            }
            $grouped[$key][] = $value;
        }
        // Recursively build a nested grouping if more parameters are supplied
        // Each grouped array value is grouped according to the next sequential key
        if (func_num_args() > 2) {
            $args = func_get_args();
            foreach ($grouped as $key => $value) {
                $params = array_merge([ $value ], array_slice($args, 2, func_num_args()));
                $grouped[$key] = call_user_func_array('array_group_by', $params);
            }
        }
        return $grouped;
    }
}

function validaCancelarReformulacaoPrazo($refid){
    global $db;
    $sql = "SELECT p.inuid, p.proid, dt.dotid 
            FROM par3.reformulacao_documento rd
            	INNER JOIN par3.documentotermo dt ON dt.dotid = rd.dotid
            	INNER JOIN par3.processo p ON p.proid = dt.proid
            WHERE rd.refid = {$refid}
            	AND rd.refidpai IS NOT NULL";
    $arDados = $db->pegaLinha($sql);
    $html = "<script>
			window.location.href = 'par3.php?modulo=principal/planoTrabalho/acompanhamento/solicitaProrrogacaoPrazo&acao=A&dotid=".$arDados['dotid']."&inuid=".$arDados['inuid']."&proid=".$arDados['proid']."';
		</script>";
    //ver($html,d);
    die($html);
    return true;
}

function validaEnvioAnalisePrazo($refid){
    global $db;
    
    $arrDados = $db->pegaLinha("SELECT refdata_solicitada, refjustificativa, refacoestomadas, refoutrasinformacoes FROM par3.reformulacao_documento WHERE refid = $refid");
    
    $boAnexo = $db->pegaUm("SELECT count(axrid) FROM par3.reformulacao_anexo WHERE refid = {$refid}");
    
    if( empty($arrDados['refdata_solicitada']) ){
        return 'O campo Data da Prorrogação é de preenchimento obrigatório!';
    } else if( empty($arrDados['refjustificativa']) ){
        return 'O campo Justificativa é de preenchimento obrigatório!';
    } else if( empty($arrDados['refacoestomadas']) ){
        return 'O campo Ações Tomadas é de preenchimento obrigatório!';
    } else if( empty($arrDados['refoutrasinformacoes']) ){
        return 'O campo Outras Informações é de preenchimento obrigatório!';
    } elseif( (int)$boAnexo == (int)0 ){
        return 'É necessário anexar um cronograma!';
    } else {
        return true;
    }
}

function validaEnvioAprovaAnalisePrazo($refid){
    global $db;
    
    $arrDados = $db->pegaLinha("SELECT refdata_aprovada, refconsideracoes, refsituacao FROM par3.reformulacao_documento WHERE refid = $refid");
    
    if( empty($arrDados['refdata_aprovada']) && $arrDados['refsituacao'] == 'A' ){
        return 'O campo Data da Aprovada é de preenchimento obrigatório!';
    } else if( empty($arrDados['refconsideracoes']) ){
        return 'O campo Parecer técnico é de preenchimento obrigatório!';
    } else if( trim($arrDados['refsituacao']) == '' ){ 
        return 'O campo Parecer é de preenchimento obrigatório!';
    } else {
        return true;
    }
}

function validaRetornoAnaliseCGPES($refid){
    global $db;
    
    $sql = "SELECT count(ae.aedid) FROM workflow.documento d
            	INNER JOIN workflow.historicodocumento hd ON hd.hstid = d.hstid
            	INNER JOIN workflow.acaoestadodoc ae ON ae.aedid = hd.aedid
            	INNER JOIN par3.reformulacao_documento rd ON rd.docid = d.docid
            WHERE rd.refid = $refid AND ae.esdidorigem in (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGPES.", ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGPES.")";
    $tem = $db->pegaUm($sql);
    if( (int)$tem > (int)0 ){
        return true;
    } else {
        return false;
    }
}

function validaRetornoAnaliseCGIMP($refid){
    global $db;
    
    $sql = "SELECT count(ae.aedid) FROM workflow.documento d
            	INNER JOIN workflow.historicodocumento hd ON hd.hstid = d.hstid
            	INNER JOIN workflow.acaoestadodoc ae ON ae.aedid = hd.aedid
            	INNER JOIN par3.reformulacao_documento rd ON rd.docid = d.docid
            WHERE rd.refid = $refid AND ae.esdidorigem in (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGIMP.", ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGIMP.")";
    $tem = $db->pegaUm($sql);
    if( (int)$tem > (int)0 ){
        return true;
    } else {
        return false;
    }
}

function validaRetornoAnaliseCGEST($refid){
    global $db;
    
    $sql = "SELECT count(ae.aedid) FROM workflow.documento d
            	INNER JOIN workflow.historicodocumento hd ON hd.hstid = d.hstid
            	INNER JOIN workflow.acaoestadodoc ae ON ae.aedid = hd.aedid
            	INNER JOIN par3.reformulacao_documento rd ON rd.docid = d.docid
            WHERE rd.refid = $refid AND ae.esdidorigem in (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_ANALISE_CGEST.", ".PAR3_REFORMULACAO_PRAZO_ESDID_EM_RECURSO_ANALISE_CGEST.")";
    $tem = $db->pegaUm($sql);
    if( (int)$tem > (int)0 ){
        return true;
    } else {
        return false;
    }
}

function download_arquivo( $arqid ){
    global $db;
    $sql = "SELECT arqnome||'.'||arqextensao FROM public.arquivo WHERE arqid = $arqid";
    $arqnome = $db->pegaUm($sql);
    return '<a style="cursor: pointer; color: blue;" onclick="download_arquivo('.$arqid.')">'.$arqnome.'</a>';
}


function importaReformulacaoOriginal($refid){
    include_once APPRAIZ . "includes/classes/Controle.class.inc";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    require_once APPRAIZ.'par3/classes/controller/Reformulacao.class.inc';
    require_once APPRAIZ.'par3/classes/model/Reformulacao.class.inc';
    $obReformulacao = new Par3_Controller_Reformulacao();
    return $obReformulacao->importaReformulacaoOriginal($refid);
}

function validaEnvioGeracaoTermoReformulacao($refid, $proid){
    global $db;
    $sql = "SELECT count(rd.refid) FROM par3.reformulacao_documento rd
            	INNER JOIN workflow.documento d ON d.docid = rd.docid
            WHERE rd.proid = $proid AND rd.refidpai IS NOT NULL AND rd.refstatus = 'A'
            	AND d.esdid IN (".PAR3_REFORMULACAO_PRAZO_ESDID_EM_GERACAO_TERMO.", ".PAR3_REFORMULACAO_ESDID_AGUARDANDO_GERACAO_TERMO.");";
    $boTem = $db->pegaUm($sql);
    
    if( (int)$boTem > (int)0 ){
        return false;
    } else {
        return true;
    }
}

function validaEnvioAnaliseReformulacao($refid){
    global $db;
    $vlr_complementacao = $db->pegaUm("SELECT sum( coalesce(ri.reiquantidade,0) * coalesce(ri.reivalor,0) ) - coalesce(dt.dotvalortermo,0)
                                        FROM par3.reformulacao_documento rd
                                        	INNER JOIN par3.reformulacao_itemcomposicao ri ON ri.refid = rd.refid AND ri.reistatus = 'A'
                                        	INNER JOIN par3.documentotermo dt ON dt.dotid = rd.dotid --AND dt.dotid = 84
                                        WHERE ri.refid = $refid
                                        	AND rd.refstatus = 'A'
                                       	GROUP BY dt.dotvalortermo");
    
    $qtd_filho = $db->pegaUm("SELECT sum(ri.reiquantidade) FROM par3.reformulacao_itemcomposicao ri WHERE ri.refid = $refid AND ri.reistatus = 'A'");
    $qtd_pai = $db->pegaUm("SELECT sum(ri.reiquantidade) FROM par3.reformulacao_itemcomposicao ri WHERE ri.refid IN (SELECT DISTINCT refidpai FROM par3.reformulacao_itemcomposicao WHERE refid = $refid AND reistatus = 'A') AND reistatus = 'A'");
    
    $msg = '';
    $refjustificativa = $db->pegaUm("SELECT refjustificativa FROM par3.reformulacao_documento WHERE refid = $refid");
    if( empty($refjustificativa) ){
        $msg .= "* É obrigatório o preenchimento da Justificativa da Reprogramação!<br>";
    }
    
    if( $qtd_filho == $qtd_pai ){
        $msg .= '* A quantidade dos itens devem ser alteradas!<br>';
    }
    
    if( (float)$vlr_complementacao > (int)0 ){
        $valorComplementarInfor = $db->pegaUm("SELECT ( coalesce(refvalorentidade,0) + coalesce(refrafmecfnde,0) ) FROM par3.reformulacao_documento WHERE refid = $refid");
        
        if( (float)$valorComplementarInfor != (float)$vlr_complementacao ){
            $msg .= '* É necessário informar o valor total do Complemento da Solicitação!<br>';
        }
    }
    
    if( !empty($msg) ){
        return '<b>Não é possivel Enviar para Análise:</b> <br>'.$msg;
    } else {
        return true;
    }
}

function validaEnvioCoordenador($refid){
    global $db;

    $sql = "SELECT trim(reftextoparecer) FROM par3.reformulacao_documento WHERE refid = $refid;";
    $reftextoparecer = $db->pegaUm($sql);
    if( empty(trim($reftextoparecer)) ){
        return 'O Texto do parecer é preenchimento obrigatório!';
    } else {
        return true;
    }
}

function cancelarReformulacao($refid){
    global $db;
    /*$sql = "UPDATE par3.reformulacao_documento SET refstatus = 'I' WHERE refid = $refid;
            UPDATE par3.reformulacao_itemcomposicao SET reistatus = 'I' WHERE refid = $refid;
            UPDATE par3.reformulacao_item_composicao_escola SET riestatus = 'I' WHERE refid = $refid;
            UPDATE par3.reformulacao_analise_itemcomposicao SET raistatus = 'I' WHERE refid = $refid;";
    $db->executar($sql);
    return $db->commit();*/
    RETURN TRUE;
}

function validaEnvioGeracaoTermo($refid){
    global $db;
    
    $sql = "SELECT coalesce(refempenhocomplementar, 0) FROM par3.reformulacao_documento WHERE refid = $refid;";
    
    $refempenhocomplementar = $db->pegaUm($sql);
    if( $refempenhocomplementar > 0 ){
        return 'Não é possivel enviar para geração de termo, existe Empenho complementar (MEC/FNDE)!';
    } else {
        return true;
    }
}

function validaEnvioComplementarEmpenho($refid){
    global $db;
    
    $sql = "SELECT coalesce(refempenhocomplementar, 0) FROM par3.reformulacao_documento WHERE refid = $refid;";    
    $refempenhocomplementar = $db->pegaUm($sql);
    if( $refempenhocomplementar == 0 ){
        return 'Não é possivel enviar para complementação de empenho, não existe Empenho complementar (MEC/FNDE)!';
    } else {
        return true;
    }
}

function formataSaldoComInformacoes($valor, $campos){
    $valorFormatado = formataNumeroMonetarioComSimbolo($valor);

    $temDadosBancarios = ( !empty($campos['dfibanco']) || !empty($campos['dfiagencia']) || !empty($campos['dficonta']) );

    if ($temDadosBancarios) {

        $html = '<div class="btn-group dropup" sytle="margin-left: 10px">';
        $html .= '<span type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown" title="Informações da Conta" aria-haspopup="true" aria-expanded="false">';
        $html .= $valorFormatado;
        $html .= ' <i style="color: #64a0e8;" class="fa fa-info-circle class_dropdown" processo="' . $campos['pronumeroprocesso'] . '"></i>';
        $html .= '</span>';
        $html .= '<div class="dropdown-menu" style="padding-bottom: 8px;">
                    <h4 class="popover-header" style="text-align: center; margin: 10px 0 0 0">Dados Bancários</h4>
                    <div style="padding: 8px 8px 0px 8px;" class="informacoes_conta">
                        <table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" style="padding: 10px; margin: 0px !important;">
                            <thead>
                                <tr>
                                    <th>Banco</th>
                                    <th>Agência</th>
                                    <th>Conta</th>
                                </tr>
                            </thead> 
                            <tbody>
                                <tr>
                                    <th>'.$campos["dfibanco"].'</th>
                                    <th>'.$campos["dfiagencia"].'</th>
                                    <th>'.$campos["dficonta"].'</th>
                                </tr>
                            </tbody>   
                        </table>
                    </div>
                  </div>
                 </div>';

    } else {
        $html = $valorFormatado;
    }

    return $html;
}

function carregaHistoricoObra($valor, $campos){
    global $db;
    
    if( $campos['tipo_obrjeto'] == 'Obra' ){        
        $html .= '<div class="btn-group dropup" sytle="margin-left: 10px">
                      <span type="button" class="btn btn-secondary dropdown-toggle" data-toggle="dropdown" title="Informações da Obra" aria-haspopup="true" aria-expanded="false">
                        ' . $valor . ' <i style="color: #64a0e8;" class="fa fa-info-circle class_dropdown" proid="'.$campos['id'].'"></i>
                      </span>
                      <div class="dropdown-menu" style="padding-bottom: 8px;">
                        <h4 class="popover-header" style="text-align: center;">Dados da Obra</h4>
                        <div style="padding: 8px 8px 0px 8px;" class="dados_obras">
                        </div>
                      </div>
                    </div>';
        
        return $html;
    } else {
        return $valor;
    }
}

function carregaDadosObraLista($proid, $mostraRetorno = true){
    global $db;
    
        
        $sql = "SELECT o.obrid AS id_par,
                	o2.obrid id_obra2,
                	es.esddsc AS situacao_par,
                	es2.esddsc AS situacao_obra2,
                	CASE WHEN val.vldstatushomologacao = 'S' THEN 'Sim' ELSE 'Não' END AS validacao,
                	(SELECT sd.sldpercobra FROM obras2.solicitacao_desembolso sd
					  		INNER JOIN workflow.documento dc ON dc.docid = sd.docid 
					  	WHERE sd.obrid = o2.obrid AND dc.esdid IN (1576, 2150) ORDER BY sldid DESC LIMIT 1) AS percet_fisico,
					((100::numeric - COALESCE(o2.obrperccontratoanterior, 0::numeric)) * COALESCE(o2.obrpercentultvistoria, 0::numeric) / 100::numeric + COALESCE(o2.obrperccontratoanterior, 0::numeric))::numeric(20,2) AS percent_obra
                FROM par3.obra o
                	INNER JOIN par3.processoobracomposicao pp ON pp.obrid = o.obrid AND pp.pocstatus = 'A'
                	INNER JOIN workflow.documento d ON d.docid = o.docid
                	INNER JOIN workflow.estadodocumento es ON es.esdid = d.esdid
                	LEFT JOIN obras2.obras o2 
                		INNER JOIN workflow.documento d2 ON d2.docid = o2.docid
                		INNER JOIN workflow.estadodocumento es2 ON es2.esdid = d2.esdid 
                	ON o2.obrid_par3 = o.obrid AND o2.obrstatus = 'A' AND o2.obridpai IS NULL
                	LEFT JOIN obras2.validacao val ON val.obrid = o2.obrid
                WHERE o.obrstatus = 'A'
                	AND pp.proid = {$proid}";
        $arDados = $db->pegaLinha($sql);
        
        if( empty($arDados['id_par']) && $mostraRetorno == false ){
            return false;
        }
        
        $html .= '<table class="table table-striped table-bordered table-hover table-condensed tabela-listagem table-responsive" style="padding: 10px; margin: 0px !important;">
                <thead>
                    <tr>
                        <th>ID (PAR3)</th>
                        <th>ID (OBRAS2)</th>
                        <th>Situação PAR3</th>
                        <th>Situação Obras2</th>
                        <th>Validação da OS</th>
                        <th>% Físico total</th>
                        <th>% Executado Instituição</th>
                    </tr>
                </thead>
                       ';
        
        if( empty($arDados['id_par']) ){
            $html .= '<tbody><tr><td colspan="7"><div class="alert alert-info text-center nenhum-registro" id="tb_render">Nenhuma registro encontrado</div></td></tr></tbody>';
        }else{
            $html .= '<tbody>
                                <tr>
                                    <td>' . $arDados['id_par'] . '</td>
                                    <td>' . $arDados['id_obra2'] . '</td>
                                    <td>' . $arDados['situacao_par'] . '</td>
                                    <td>' . $arDados['situacao_obra2'] . '</td>
                                    <td>' . $arDados['validacao'] . '</td>
                                    <td>' . $arDados['percet_fisico'] . '</td>
                                    <td>' . $arDados['percent_obra'] . '</td>
                                </tr>
                          </tbody>';
        }
        $html .= '</table>';
        
        return $html;
}

function aprovaEnvioProrrogacaoOficio( $dotid, $proid ){
    global $db;
    
    $sql = "update par3.documentotermo set dotstatus = 'I' where dotid in (
            	select dotidpai from par3.documentotermo where proid = {$proid} and dotid = {$dotid}
            )";
    $db->executar($sql);
    
    $db->executar("update par3.documentotermo set dotstatus = 'A' where dotid = $dotid");
    
    return $db->commit();
}

function trataEnvioValidacaoGestor($refid, $dotid, $proid){
    global $db;
    
    /*$dotid = $db->pegaUm("SELECT dotid FROM par3.reformulacao_documento WHERE proid = $proid AND refid = $refid;");
    $db->executar($sql);*/
    $db->executar("update par3.documentotermo set dotstatus = 'I' where dotid = $dotid");
    
    return $db->commit();
}

function validaEnvioValidacaoCoordenadorReformulacao($refid){
    global $db;
    
    $sql = "SELECT sum(ri.reiquantidade) FROM par3.reformulacao_itemcomposicao ri WHERE ri.refid = (SELECT refidpai FROM par3.reformulacao_documento WHERE refid = $refid) AND ri.reistatus = 'A'";
    $qtd_item = $db->pegaUm($sql);
    
    $sql = "SELECT sum(raiqtdaprovado) FROM par3.reformulacao_analise_itemcomposicao WHERE refid = $refid";
    $qtd_analise = $db->pegaUm($sql);
    
    if( $qtd_item == $qtd_analise ){
        return 'Não é possivel Enviar para validação do Coordenador. Análise não realizada!';
    } else {
        return true;
    }
}

function mostraPorcentagemPagamento($valor,$params)
{
//    $valor_fnde = par3_mascaraMoeda($params['valor_fnde'],false,true);
    $pagamento    = par3_mascaraMoeda($params['vlrpago'],false,true);
    $percentEmpenho   = $params['percent_empenho'];
    $percentPagamento = $params['percent_pagamento'];

    $content .= "<table class='table table-responsive table-bordered'>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Porcentagem Empenho: </b></td><td>{$percentEmpenho}</td></tr>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Porcentagem Pagamento: </b></td><td>{$percentPagamento}</td></tr>";
    $content .= '</table>';


    $html ="<a data-toggle='popover' class='popoverpagamentopercent' data-html='true' title='Porcentagem Empenho Valor FNDE' data-trigger='hover' data-content=\"".$content."\" data-placement='auto right'>{$pagamento}</a>";
    return $html;
}

function mostraValoresObra($valor,$params)
{
    $valor_fnde             = par3_mascaraMoeda($params['valor_fnde'],false,true);
    $obrvalor_contrapartida = par3_mascaraMoeda($params['obrvalor_contrapartida'],false,true);
    $obra                   = par3_mascaraMoeda($params['obrvalor'],false,true);
    $content .= "<table class='table table-responsive table-bordered'>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Valor da Obra: </b></td><td>{$obra}</td></tr>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Valor de Contrapartida: </b></td><td>{$obrvalor_contrapartida}</td></tr>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Valor FNDE: </b></td><td>{$valor_fnde}</td></tr>";
    $content .= '</table>';


    $html ="<a data-toggle='popover' class='popoverfnde' data-html='true' title='Valor FNDE' data-trigger='hover' data-content=\"".$content."\" data-placement='auto right'>{$valor_fnde}</a>";
    return $html;
}

function mostrarPopupTermo($valor, $params)
{
    $dotdatafimvigencia = (new DateTime($params['dotdatafimvigencia']))->format('d/m/Y H:i:s');
    $content .= "<table class='table table-responsive table-bordered'>";
    $content .= "<tr><td class='info' style='vertical-align: middle;'><b>Data de Vigência: </b></td><td>{$dotdatafimvigencia}</td></tr>";
    $content .= '</table>';


    $html ="<a data-toggle='popover' class='termo_detalhe' data-html='true' title='Data de Vigência' data-trigger='hover' data-content=\"".$content."\" data-placement='auto right'><span class='dotid' value='{$params['dotid']}'>".$valor."</span></a>";
    return $html;
}
//return '<a class="termo_detalhe"><span class="dotid" value='.$arCampos['dotid'].'>'.$termo.'</span></a>';
