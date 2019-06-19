<?php

function getScriptsAdStyles() {
        return <<<STR
        <script language="JavaScript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>


STR;
    }
function verificaBloqueioEdicao($obrid){
        $objObras = new Obras();
        $objObras->carregarPorIdCache($obrid);

        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            $blockEdicao = true;
        }else{
            $blockEdicao = false;
        }

    return $blockEdicao;
}



function atualizaDocidNullRestricao($rstid){
    $restricao = new Restricao();
    $resp = $restricao->atualizaDocidNullRetricao($rstid);

    if($resp){
        die("<script>
                alert('Documento atualizado com sucesso.');
                window.parent.opener.getLista('visual');
                window.close();
             </script>");
    }else{
        die("<script>
                alert('Documento não pode ser atualizado.');
                window.close();
             </script>");
    }
}

function montaLista($tipo){
    global $db;
    $restricao = new Restricao();
    $sql       = $restricao->getDadosListaRestricao('sql', $tipo);

    $cabecalho = array( "Ação", "ID Obra", "ID Item", "Item", "Providência", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo",
                        "Nome da Obra", "Situação Obra", "Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Último Tramite", "Data do Último Tramite");

    //ini xls
    if($tipo == 'xls'){

        $dados = $db->carregar($sql);

        $dados = (!$dados) ? array() : $dados;

        foreach ($dados as $key => $value){
            $historico = explode(';', $value['historico']);
            $dados[$key] = $dados[$key] + $historico;
            unset($dados[$key]['historico']);

            $contador[] = count($historico);

        }

             $i = 1;
        if($contador) {
            while ($i <= (max($contador) / 3 + 1)) {
                $cabecalho_extra[] = "Data {$i}";
                $cabecalho_extra[] = "Ação {$i}";
                $cabecalho_extra[] = "Usuário {$i}";
                $i++;
            }

        }

        $cabecalho = array( "ID Obra", "ID Item", "Item", "Situação", "Estado", "Município", "Esfera", "Fase", "Tipo", "Tipologia",
                                "Nome da Obra", "Situação","Data Cadastro", "Criado por", "Previsão da Providência", "Superação", "Superado por", "Usuário da Operação", "Data da Última Tramitação","Descrição","Providência","Divisão","Item");
        ini_set("memory_limit", "7000M");

        if(!empty($cabecalho_extra)){

        $novo_cabecalho = array_merge($cabecalho,$cabecalho_extra);
        }
        else{

             $novo_cabecalho = $cabecalho;
        }

        $db->sql_to_xml_excel ($dados, 'ListadeRestriçãoeInconformidade', $novo_cabecalho);
    }else{

        $db->monta_lista($sql, $cabecalho, 30, 10,'N','center', 'N', 'N');
    }//end xls

}



function getSqlEstadoDoc(int $tpdid) :string {
	return <<<SQL
	SELECT esdid as codigo, esddsc as descricao
                            FROM workflow.estadodocumento
                            WHERE tpdid='{$tpdid}'
                              AND esdstatus='A'
                            ORDER BY esdordem
SQL;

}
