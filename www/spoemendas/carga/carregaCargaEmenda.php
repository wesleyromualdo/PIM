<?php
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/library/simec/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emenda.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/EmendaDetalhe.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emendadetalheentidade.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Entidadebeneficiada.class.inc";
include_once APPRAIZ . "spoemendas/classes/model/Emendadetalheptres.php";
include_once APPRAIZ . "monitora/classes/model/Acao.php";

function atualizarEmenda($emeid, $acaid)
{
    // -- Atualizando o acaid da emenda
    $emenda = new Spoemendas_Model_Emenda($emeid);
    $emenda->acaid = $acaid;

    $emenda->salvar([
        'emeano',
        'emevalor',
        'emecod',
        'autid',
        'acaid',
        'resid',
        'emetipo',
        'emeano',
        'emedescentraliza',
        'emelibera',
        'emevalor',
        'etoid',
        'emerelator'
    ]);
}

function salvarEmendaDetalhe($emeid, array $dados)
{

    ver($emeid, $dados, d);

    // -- Verifica se tem um conjunto de emendadetalhe para a emenda
    $dadosEmDetalhe = (new Spoemendas_Model_EmendaDetalhe())
        ->carregarPorEmeidEComposicao(
            $emeid,
            $dados['gnd'],
            $dados['fonte'],
            $dados['mapcod']
        );

    if ($dadosEmDetalhe) { // -- Update
        $modelEmDetalhe = new Spoemendas_Model_EmendaDetalhe($dadosEmDetalhe['emdid']);
        $modelEmDetalhe->emdvalor = $dados['dotacaoatualizada'];
        $modelEmDetalhe->usucpfalteracao = $_SESSION['usucpf'];
        $modelEmDetalhe->emddataalteracao = date('Y-m-d H:i:s');
        $modelEmDetalhe->salvar();

        return;
    }
//    die('WTN?? <o>');
}

function atualizarEmendaDetalhe($emdid, $gndcod, $foncod, $mapcod)
{
    $modelEmDet = new Spoemendas_Model_EmendaDetalhe($emdid);
    $modelEmDet->gndcod = $gndcod;
    $modelEmDet->foncod = $foncod;
    $modelEmDet->mapcod = $mapcod;
    $modelEmDet->emdstatus = 'A';

    $modelEmDet->salvar();
}





//function salvarEmendaDetalheEntidade(array $dados, $emdid, $edeid)
//{
//    if (!is_null($edeid)) {
//        $modelEmDetEntidade = new Spoemendas_Model_Emendadetalheentidade($edeid);
//    } else {
//        $modelEmDetEntidade = new Spoemendas_Model_Emendadetalheentidade();
//        $modelEmDetEntidade->emdid = $emdid;
//        $modelEmDetEntidade->enbid = $enbid;
//    }
//
//    $modelEmDetEntidade->edevalor = $dados['valor_rcl_apurada'];
//    $modelEmDetEntidade->edeprioridade = $dados['prioridade'];
//    $modelEmDetEntidade->edelimiteempenhobeneficiario = $dados['limiteempenhado'];
//    $modelEmDetEntidade->edestatus = 'A';
//    $modelEmDetEntidade->usucpfalteracao = $_SESSION['usucpf'];
//    $modelEmDetEntidade->ededataalteracao = date('Y-m-d H:i:s');
//
//    return $modelEmDetEntidade->salvar();
//}



/*
//function atualizarDetalheEmenda($emeid, array $dados)
//{
//        // Carrega o código do detalhe da emenda
//        $sql = <<<SQL
//            select
//              emdid
//            from emenda.emendadetalhe
//            where foncod = '{$linha['fonte']}'
//            and emeid = {$emeid}
//SQL;
//
//        $emdid = $db->pegaUm($sql);
//
//        $detalhe = new Spoemendas_Model_EmendaDetalhe();
//        $edp = new Spoemendas_Model_Emendadetalheptres();
//
//        if (!empty($emdid)) {
//            $detalhe->carregarPorId($emdid);
//        } else {
//            $detalhe->foncod = $linha['fonte'];
//            $detalhe->emeid = $emeid;
//            $detalhe->emdtipo = 'E';
//            $detalhe->emdimpositiva = $linha['rp_atual'];
//            $detalhe->emdvalor = 0;
//        }
//        $detalhe->gndcod = $linha['gnd'];
//        $detalhe->mapcod = $linha['mapcod'];
//
//        $detalhe->salvar();
//        $emdid = $detalhe->emdid;
//
//
//        $sql = <<<SQL
//          select edpid from spoemendas.emendadetalheptres where emdid = {$emdid} and ptres = '{$linha['ptres']}' and plocod = '{$linha['planoorcamentario']}'
//SQL;
//        $edpid = $db->pegaUm($sql);
//
//        if(empty($edpid)) {
//            $edp->emdid = $emdid;
//            $edp->ptres = $linha['ptres'];
//            $edp->plocod = $linha['planoorcamentario'];
//        } else {
//            $edp->carregarPorId($edpid);
//        }
//
//        $edp->emdvalor = $linha['dotacaoatualizada'];
//        $edp->salvar();
//
//
//        $detalhe->emdvalor = str_replace('.', ',', $edp->getTotalValor($emdid));
//        $detalhe->emddataalteracao = date('Y-m-d H:i:s');
//        $detalhe->usucpfalteracao = $_SESSION['usucpf'];
//
//        $detalhe->salvar();
//        $emdiddavez = $emdid;
//}
*/

function CNPJComfonteModalidadeGNDSemAlteracao(&$banco, &$carga)
{
    foreach ($banco as &$_banco) {

        ver($_banco['enbcnpj'], $_banco['gndcod'], $_banco['foncod'], $_banco['mapcod']);
        ver($carga['cnpj'], $carga['gnd'], $carga['fonte'], $carga['mapcod'], d);


        if (($_banco['enbcnpj'] == $carga['cnpj'])
            && ($_banco['gndcod'] == $carga['gnd'])
            && ($_banco['foncod'] == $carga['fonte'])
            && ($_banco['mapcod'] == $carga['mapcod'])
        ) {
            $_banco['temcorrespondencia'] = true;
            $carga['edeid'] = $_banco['edeid'];
            return true;
        }
    }


    return false;
}

function DeUmConjuntoParaOutro($benefGMFBanco, $benefGMFCarga)
{
    $arrConjuntos = [];
    foreach ($benefGMFBanco as $beneficiario) {
        $arrConjuntos[] = "{$beneficiario['gndcod']}{$beneficiario['mapcod']}{$beneficiario['foncod']}";
    }

    foreach ($benefGMFCarga as $beneficiario) {
        $arrConjuntos[] = "{$beneficiario['gnd']}{$beneficiario['mapcod']}{$beneficiario['fonte']}";
    }

    return 1 == count(array_flip($arrConjuntos));
}




function atualizarBeneficiario($emeid, $edeid, $dados)
{

    $modelEmDetEntidade;



//    $dadosBeneficiario = (new Spoemendas_Model_Entidadebeneficiada())
//        ->carregarDetalhesBeneficiarios(
//            $emeid,
//            $dados['cnpj']
//        );
//
//    if (empty($dadosBeneficiario)) {
//        echo "Beneficiário não encontrado({$dados['cnpj']}@{$dados['emenda']}/{$dados['ano_emenda']});\n";
//        ver($carga);
//
///*
////                $enb = new Spoemendas_Model_Entidadebeneficiada();
////                $enb->enbcnpj = $linha['cnpj'];
////                $enb->enbano = $linha['ano_emenda'];
////                $enb->enbnome = str_replace($linha['cnpj'].' - ', '', $linha['nome_beneficiario']);
////
////                $enb->salvar();
////                $enbid = $enb->enbid;
////                ver($enb);
//*/
//        return;
//    }
//
//    // -- Beneficiario sem alteracao de detalhe (FON/MOD/GND)
//    if (fonteModalidadeGNDSemAlteracao($dadosBeneficiario, $dados)) {
//        salvarEmendaDetalheEntidade($dados, null, null, $dadosBeneficiario['edeid']);
//        return;
////    } else {
////        // -- Current não funfou?? oO
////        $dadosBeneficiario = $dadosBeneficiario[0];
//    }
//
//    // -- Beneficiario com alteracao de detalhe ou com um novo detalhe
//    $dadosBeneficiario = (new Spoemendas_Model_Emenda())
//        ->carregarBeneficiariosPorEmeid($emeid, $dados['cnpj']);
//
//    ver($dadosBeneficiario, d);
//
//
//
//
//
//
////    ver($dadosBeneficiario, d);
//
//
////
//
////
////    // -- Verificando se já existe um detalhe para aninhar o beneficiario
////    $dadosEmDetalhe = (new Spoemendas_Model_EmendaDetalhe())
////        ->carregarPorEmeidEComposicao(
////            $emeid,
////            $dados['gnd'],
////            $dados['fonte'],
////            $dados['mapcod']
////        );
////
////    // -- Não existe o detalhe, cadastrando um novo
////    if (empty($dadosEmDetalhe)) {
////        $modelEmDetalhe = new Spoemendas_Model_EmendaDetalhe();
////        $modelEmDetalhe->emeid = $emeid;
////        $modelEmDetalhe->gndcod = $dados['gnd'];
////        $modelEmDetalhe->foncod = $dados['fonte'];
////        $modelEmDetalhe->mapcod = $dados['mapcod'];
////        $modelEmDetalhe->emdtipo = 'E';
////        $modelEmDetalhe->emdimpositiva = $dados['rp_atual'];
////        $modelEmDetalhe->emdvalor = $emevalor;
////        $modelEmDetalhe->usucpfalteracao = $_SESSION['usucpf'];
////        $modelEmDetalhe->emddataalteracao = date('Y-m-d H:i:s');
////
////        $emdid = $modelEmDetalhe->salvar();
////    }
////
////    // -- Associando o beneficiario ao detalhe
////    salvarEmendaDetalheEntidade(
////        $dados,
////        $emdid?$emdid:$dadosEmDetalhe['emdid'],
////        $enbid?$enbid:$dadosBeneficiario['enbid']
////    );
}

function atualizarBeneficiarios($emeid, $beneficiarios)
{
    $benefGMFCarga = [];

    // -- Não posso tirar isso? É só agrupar nesse formato no objeto primário
    foreach ($beneficiarios as $beneficiario) {
        foreach ($beneficiario as $dadosBeneficiario) {
            $benefGMFCarga[] = $dadosBeneficiario;
        }
    }

    $benefGMFBanco = (new Spoemendas_Model_Emenda())
        ->carregarBeneficiariosPorEmeid($emeid, null);


    ver($benefGMFBanco, $benefGMFCarga, d);



    $benefCMFSemCorrespondencia = [];

    // -- Atualizando todos que estão iguais
    foreach ($benefGMFCarga as &$benefGMF) {
        if (CNPJComfonteModalidadeGNDSemAlteracao($benefGMFBanco, $benefGMF)) {
            atualizarEmendaDetalheEntidade($benefGMF['edeid'], $benefGMF);
        } else {
            $benefCMFSemCorrespondencia[] = $benefGMF;
        }
    }

    // -- Todos os beneficiarios sao iguais - finaliza
    if (empty($benefCMFSemCorrespondencia)) {
        return;
    }

    ver($benefCMFSemCorrespondencia, $benefGMFBanco, d);


    // -- Atualizando aqueles que tem EXATAMENTE um detalhe e mudam para um OUTRO detalhe
    if (DeUmConjuntoParaOutro($benefGMFBanco, $benefGMFCarga)) {
        atualizarEmendaDetalhe(
            $benefGMFBanco[0]['emdid'],
            $benefGMFCarga[0]['gnd'],
            $benefGMFCarga[0]['fonte'],
            $benefGMFCarga[0]['mapcod']
        );

//        die('hiha!');
        atualizarBeneficiarios($emeid, $beneficiarios);

    }

//die();


    ver($benefCMFCargaComMudanca);

    ver(':P', d);







}





// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';

$db = new cls_banco();

// Carrega os dados da carga
$sql = <<<SQL
    select
      esfera,
      subfuncao,
      funcao,
      acao,
      localizador,
      programa,
      uo,
      emenda,
      ano_emenda,
      autcod,
      planoorcamentario,
      rp_atual,
      gnd,
      fonte,
      mapcod,
      dotacaoatualizada,
      cnpj,
      nome_beneficiario,
      uf,
      valor_rcl_apurada,
      valor_disponivel,
      prioridade,
      valorimpedido,
      ptres,
      limiteempenhado
    from emenda.carga_emenda_siop
    where ano_emenda = '2017'
      and emenda = '19830020'
      AND emenda NOT IN('26780004', '30640004', '37930013')
      --and gnd = 4
    order by emenda, ano_emenda, gnd, mapcod, cnpj desc
SQL;

$arrCarga = $db->carregar($sql);
$arrCarga = $arrCarga ? $arrCarga : [];


//ver($arrCarga, d);

$inativaBeneficiario = true;
$emendaatual = '';


$listaEmendas = [];
foreach ($arrCarga as $linha) {

    if (array_key_exists($linha['emenda'], $listaEmendas)) {
        if (!empty($linha['cnpj'])) {
            $listaEmendas[$linha['emenda']]['beneficiarios'][$linha['cnpj']]["{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}"]
            = [
                'nome_beneficiario' => str_replace("{$linha['cnpj']} - ", '', $linha['nome_beneficiario']),
                'gnd' => $linha['gnd'],
                'fonte' => $linha['fonte'],
                'mapcod' => $linha['mapcod'],
                'uf' => $linha['uf'],
                'valor_rcl_apurada' => $linha['valor_rcl_apurada'],
                'cnpj' => $linha['cnpj'],
                'valor_disponivel' => $linha['valor_disponivel'],
                'prioridade' => $linha['prioridade'],
                'valorimpedido' => $linha['valorimpedido'],
                'limiteempenhado' => $linha['limiteempenhado'],
            ];
        } else {
            $listaEmendas[$linha['emenda']]['detalhes']["{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}"]
                = $linha['dotacaoatualizada'];
        }
    } elseif (empty($linha['cnpj'])) {
        $listaEmendas[$linha['emenda']] = [
            'acao' => $linha['acao'],
            'localizador' => $linha['localizador'],
            'programa'=> $linha['programa'],
            'uo'=> $linha['uo'],
            'ptres'=> $linha['ptres'],
            'planoorcamentario'=> $linha['planoorcamentario'],
/*
//            'emenda'=> $linha['emenda'],
//            'ano_emenda'=> $linha['ano_emenda'],
*/
            'rp_atual' => $linha['rp_atual'],
            'beneficiarios' => [],
            'detalhes' => [
                "{$linha['gnd']}-{$linha['fonte']}-{$linha['mapcod']}" => $linha['dotacaoatualizada']
            ]
        ];
    }

/* v2
//    // -- Verifica se a linha da carga é referente à um beneficiário
//    if (empty($linha['cnpj'])) {
//
//        // -- Procurando o ID da emenda
//        $emeid = $modelEmendas
//            ->carregarPorCodigo($linha['emenda'], $linha['ano_emenda']);
//        $emevalor = $linha['dotacaoatualizada'];
//
//        if (empty($emeid)) {
//            echo "Emenda não encontrada({$linha['emenda']});\n";
//            ver($linha);
//            continue;
//        }
//
//        // -- Procurando o ID da ação
//        $acaid = $modelAcao->carregarPorProgramatica(
//            $linha['uo'],
//            $linha['programa'],
//            $linha['acao'],
//            $linha['localizador'],
//            $linha['ano_emenda']
//        );
//
//        if (empty($acaid)) {
//            echo "Ação não encontrada({$linha['emenda']} - {$linha['uo']}.{$linha['programa']}.{$linha['acao']}.{$linha['localizador']}'/{$linha['ano_emenda']});\n";
//            ver($linha);
//            continue;
//        }
//
//        atualizarEmenda($emeid, $acaid, $linha);
//        continue;
//    }
//
//    ver($linha);
//
//    // -- Atualiza beneficiário
//    atualizarBeneficiario($emeid, $emevalor, $linha);
//
//    // -- fim!
//    $db->commit();
*/
}

$exercicio = '2017';
$modelEmendas = new Spoemendas_Model_Emenda();
$modelAcao = new Monitora_Model_Acao();

foreach ($listaEmendas as $emecod => $emenda) {
    // -- Procurando o ID da emenda
    $emeid = $modelEmendas
        ->carregarPorCodigo($emecod, $exercicio);

    if (empty($emeid)) {
        echo "Emenda não encontrada({$emecod});\n";
        ver($emecod, $emenda);
        continue;
    }

    // -- Procurando o ID da ação
    $acaid = $modelAcao->carregarPorProgramatica(
        $emenda['uo'],
        $emenda['programa'],
        $emenda['acao'],
        $emenda['localizador'],
        $exercicio
    );

    if (empty($acaid)) {
        echo "Ação não encontrada({$emenda['emenda']} - {$emenda['uo']}.{$emenda['programa']}.{$emenda['acao']}.{$emenda['localizador']}'/{$emenda['ano_emenda']});\n";
        ver($emecod, $emenda);
        continue;
    }

    atualizarEmenda($emeid, $acaid);

    atualizarBeneficiarios($emeid, $emenda['beneficiarios']);

//    atualizarDetalhes($emeid, $emenda['detalhes']);
}

$db->commit();

ver ($listaEmendas, d);




    // ------------------------------------------------------------------------------------------------------------------------------------------------------------
/*

//
//    if(!empty($emeid)) {
//
//        // Carrega o código do detalhe da emenda
//        $sql = <<<SQL
//            select
//              emdid
//            from emenda.emendadetalhe
//            where foncod = '{$linha['fonte']}'
//            and emeid = {$emeid}
//SQL;
//
//        $emdid = $db->pegaUm($sql);
//
//        $detalhe = new Spoemendas_Model_EmendaDetalhe();
//        $edp = new Spoemendas_Model_Emendadetalheptres();
//
//        if (!empty($emdid)) {
//            $detalhe->carregarPorId($emdid);
//        } else {
//            $detalhe->foncod = $linha['fonte'];
//            $detalhe->emeid = $emeid;
//            $detalhe->emdtipo = 'E';
//            $detalhe->emdimpositiva = $linha['rp_atual'];
//            $detalhe->emdvalor = 0;
//        }
//        $detalhe->gndcod = $linha['gnd'];
//        $detalhe->mapcod = $linha['mapcod'];
//
//        $detalhe->salvar();
//        $emdid = $detalhe->emdid;
//
//        if(empty($linha['cnpj'])) {
//
//            $sql = <<<SQL
//              select edpid from spoemendas.emendadetalheptres where emdid = {$emdid} and ptres = '{$linha['ptres']}' and plocod = '{$linha['planoorcamentario']}'
//SQL;
//            $edpid = $db->pegaUm($sql);
//
//            if(empty($edpid)) {
//                $edp->emdid = $emdid;
//                $edp->ptres = $linha['ptres'];
//                $edp->plocod = $linha['planoorcamentario'];
//            } else {
//                $edp->carregarPorId($edpid);
//            }
//
//            $edp->emdvalor = $linha['dotacaoatualizada'];
//            $edp->salvar();
//
//
//            $detalhe->emdvalor = str_replace('.', ',', $edp->getTotalValor($emdid));
//            $detalhe->emddataalteracao = date('Y-m-d H:i:s');
//            $detalhe->usucpfalteracao = $_SESSION['usucpf'];
//
//            $detalhe->salvar();
//            $emdiddavez = $emdid;

//        } else {
//            // Carrega o beneficiário
//            if($linha['rp_atual'] != 7 && $linha['rp_atual'] != 2 && $emendaatual != $linha['emenda']){
//                $sql = <<<SQL
//                    UPDATE emenda.emendadetalheentidade SET
//                          edestatus = 'I'
//                    WHERE edeid in (
//                          SELECT
//                                edeid
//                          FROM emenda.emendadetalheentidade ede
//                          INNER JOIN emenda.emendadetalhe emd USING (emdid)
//                          INNER JOIN emenda.emenda eme USING (emeid)
//                          WHERE emecod = '{$linha['emenda']}'
//                    )
//SQL;
//                $db->executar($sql);
//                $emendaatual = $linha['emenda'];
//            }
//
//            $sql = <<<SQL
//              SELECT enbid FROM emenda.entidadebeneficiada where enbcnpj = '{$linha['cnpj']}' and enbano = '{$linha['ano_emenda']}'
//SQL;
//            $enbid = $db->pegaUm($sql);
//
//            if(empty($enbid)) {
//                $enb = new Spoemendas_Model_Entidadebeneficiada();
//
//                $enb->enbcnpj = $linha['cnpj'];
//                $enb->enbano = $linha['ano_emenda'];
//                $enb->enbnome = str_replace($linha['cnpj'].' - ', '', $linha['nome_beneficiario']);
//
//                $enb->salvar();
//                $enbid = $enb->enbid;
//                ver($enb);
//            }
//            $sql = <<<SQL
//              SELECT edeid FROM emenda.emendadetalheentidade WHERE emdid = {$emdid} AND enbid = {$enbid}
//SQL;
//            $edeid = $db->pegaUm($sql);
//
//            $ede = new Spoemendas_Model_Emendadetalheentidade();
//            if(!empty($edeid)){
//                $ede->carregarPorId($edeid);
//            } else {
//                $ede->emdid = $emdid;
//                $ede->enbid = $enbid;
//            }
//            $ede->edeprioridade = $linha['prioridade'];
//            $ede->edelimiteempenhobeneficiario = $linha['limiteempenhado'];
//            $ede->edevalor = $linha['valor_rcl_apurada'];
//            $ede->edestatus = 'A';
//
//            $ede->salvar();
//        }
//    }
//    if(!$db->commit()){
//        throw new Exception('Erro ao realizar a carga');
//    };

//}
*/
// -- MANUTENÇÕES --------------------------------------------------------------------------------------------------------------
// -- Atualização do emenda.emevalor
$sql = <<<SQL
SELECT DISTINCT ano_emenda
  FROM emenda.carga_emenda_siop
SQL;

$exercicioCarga = $db->pegaUm($sql);
$sql = <<<DML
UPDATE emenda.emenda
  SET emevalor = (SELECT SUM(emdvalor)
                    FROM emenda.emendadetalhe emd
                    WHERE emd.emeid = emenda.emeid
                      AND emd.emdstatus = 'A')
  WHERE emeano = '{$exercicioCarga}'
DML;

$db->executar($sql);
$db->commit();




//

//$arrCarga = $arrCarga ? $arrCarga : [];
//
//foreach ($arrCarga as $linha) {
//    $sql = <<<SQL
//      SELECT emeid FROM emenda.emenda WHERE emecod = '{$linha['emenda']}' AND emeano = '{$linha['ano_emenda']}'
//SQL;
//    $emeid = $db->pegaUm($sql);
//
//    if(!empty($emeid)) {
//        $sql = <<<SQL
//      SELECT sum(emdvalor) FROM emenda.emendadetalhe WHERE emeid = {$emeid}
//SQL;
//        $emevalor = $db->pegaUm($sql);
//
//        $emenda = new Spoemendas_Model_Emenda($emeid);
//        $emenda->emevalor = $emevalor;
//        $emenda->salvar();
//        if (!$db->commit()) {
//            throw new Exception('Erro ao realizar a carga');
//        }
//    }
//};
