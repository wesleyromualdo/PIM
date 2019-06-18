<?php

/**
 * Processamento de requisições de beneficiários.
 *
 * @version $Id: beneficiarios-request.inc 112692 2016-08-08 18:13:27Z werteralmeida $
 */

/**
 * Consulta os dados de um CNPJ na receita federal.
 *
 * @param string $cnpj Número do CNPJ para consulta
 * @return mixed[]
 */
function findCNPJ_RF($cnpj) {
    require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';
    $pessoaJuridica = new PessoaJuridicaClient('http://ws.mec.gov.br/PessoaJuridica/wsdl');

    $pj = str_replace(array('/', '.', '-'), '', $cnpj);
    $xml = $pessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
    $obj = (object) simplexml_load_string($xml);

    if ($obj->PESSOA->ERRO) {
        $return = 'CNPJ inexistente na base da Receita Federal.';
    } else {
        $return = [
            'nome' => $obj->PESSOA->no_empresarial_rf,
            'muncod' => (int) $obj->PESSOA->ENDERECOS->ENDERECO->co_cidade,
            'estuf' => $obj->PESSOA->ENDERECOS->ENDERECO->sg_uf
        ];
    }

    return $return;
}

$ignoraDocid = false;
if (!isset($requisicao)) { // -- Requisicao de principal/alteraremendas/editaremenda
    $requisicao = $_REQUEST['requisicao'];
    $ignoraDocid = true;
}

switch ($requisicao) {
    case 'apagarBeneficiario':
        $solicitacao = new Spoemendas_Model_Emendadetalheentidade($_REQUEST['edeid']);
        $solicitacao->popularDadosObjeto();
        if ($solicitacao->salvar()) {
            $solicitacao->commit();
            $fm->addMensagem('Entidade Beneficiada apagada com sucesso.', Simec_Helper_FlashMessage::SUCESSO);
        } else {
            $solicitacao->rollback();
            $fm->addMensagem('Erro ao apagar', Simec_Helper_FlashMessage::ERRO);
        }
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
    //Pesquisa CNPJ na RECEITA FEDERAL
    case 'pesquisar_cnpj':
        $cnpjReceita = findCNPJ_RF($_REQUEST['cnpj']);
        if (is_array($cnpjReceita)) {
            echo $cnpjReceita['nome'];
        } else {
            echo $cnpjReceita;
        }
        die;
    case 'nova_entidade_beneficiada':
        $dados = $_REQUEST['cadastrobeneficiario'];

        // -- Verifica se existe a entidade beneficiada
        $entidadeBeneficiada = new Spoemendas_Model_Entidadebeneficiada();
        $dadosEntidade = $entidadeBeneficiada->findCNPF($dados['enbcnpj']);

        if ($dadosEntidade) {
            $emendaSalva = salvarEmendaDetalheEntidade([
                'enbid' => $dadosEntidade['enbid'],
                'edevalor' => $_REQUEST['cadastrobeneficiario']['edevalor'],
                'emdid' => $_REQUEST['cadastrobeneficiario']['emdid'],
                'edeid' => $_REQUEST['cadastrobeneficiario']['edeid']
            ]);
            /* Reativa a entidade, caso já tenha sido apagada uma vez */
            $dadosEntidade['enbstatus'] = 'A';
            $entidadeBeneficiada->popularDadosObjeto($dadosEntidade);
            $entidadeBeneficiada->salvar();
            $entidadeBeneficiada->commit();
        } else {
            $cnpjReceita = findCNPJ_RF($dados['enbcnpj']);

            if (!is_array($cnpjReceita)) {
                $cnpjReceita = [
                    'nome' => 'Não identificado',
                    'muncod' => null,
                    'estuf' => null
                ];
            }

            $dadosEntidadeBeneficiada = array();
            $dadosEntidadeBeneficiada['enbcnpj'] = $dados['enbcnpj'];
            $dadosEntidadeBeneficiada['enbnome'] = $cnpjReceita['nome'];
            $dadosEntidadeBeneficiada['muncod'] = $cnpjReceita['muncod'];
            $dadosEntidadeBeneficiada['estuf'] = $cnpjReceita['estuf'];
            $dadosEntidadeBeneficiada['enbstatus'] = 'A';
            $entidadeBeneficiada->popularDadosObjeto($dadosEntidadeBeneficiada);
            if ($enbid = $entidadeBeneficiada->salvar()) {
                $entidadeBeneficiada->commit();

                $emendaSalva = salvarEmendaDetalheEntidade([
                    'enbid' => $enbid,
                    'edevalor' => $_REQUEST['cadastrobeneficiario']['edevalor'],
                    'emdid' => $_REQUEST['cadastrobeneficiario']['emdid'],
                    'edeid' => $_REQUEST['cadastrobeneficiario']['edeid']
                ]);
            }
        }

        if ($emendaSalva) {
            if (!$ignoraDocid) {
                cadastrarDocid($informacoes, $infoemenda, $_REQUEST['emeid']);
            }
            $fm->addMensagem('Requisição realizada com sucesso.');
        } else {
            $fm->addMensagem('Não foi realizar sua requisicao', Simec_Helper_FlashMessage::ERRO);
        }
        header("Location: {$_SERVER['REQUEST_URI']}");
        die();
}

/**
 * Emendadetalheentidade só deve ser receber um novo registro, apenas se não existir um conjunto enbid e emdid.
 *
 * @param mixed[] $dados Para criação / atualização da emendadetalheentidade.
 * @return boolean
 */
function salvarEmendaDetalheEntidade($dados) {
    $dados['edevalor'] = str_replace(['.', ','], ['', '.'], $dados['edevalor']);
    $dados['edestatus'] = 'A';
    $dados['usucpfalteracao'] = $_SESSION['usucpf'];


    $detalhe = new Spoemendas_Model_Emendadetalheentidade();
    
    if ($dados['enbid'] && $dados['emdid']) {
        $dadosDetalheEntidade = $detalhe->findBy([
            'enbid' => $dados['enbid'],
            'emdid' => $dados['emdid'],
            'edestatus' => 'A'
        ]);
    }
    
    if ($dadosDetalheEntidade) {
        $dados['edeid'] = current($dadosDetalheEntidade)['edeid'];
    }

    $detalhe->popularDadosObjeto($dados);
    #ver($dados,d,$detalhe->salvar());
    if ($detalhe->salvar()) {
        $detalhe->commit();
        return true;
    }
    return false;
}
