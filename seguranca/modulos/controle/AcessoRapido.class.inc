<?php

class AcessoRapido extends mvc\Controle
{
    public function iniciarFluxo()
    {
        switch ($_REQUEST['requisicao']) {
            case 'buscarPerfil':
                $this->buscarPerfil();
                die;
            case 'buscarListaAcessoRapido':
                $this->buscarListaAcessoRapido();
                die;
            case 'salvar':
                $this->salvar();
                $this->sucesso();
                die;
            case 'editar':
                $this->buscarDadoEdicao();
                die;
            case 'inativar':
                $this->inativar();
                die;
        }
    }
    
    public function acaoCadastro()
    {
        require_once APPRAIZ. 'includes/classes/modelo/seguranca/Sistema.class.inc';
        
        $arDado = array();
        $sistema = new Sistema();
        $arDado['comboModulo'] = $sistema->comboModulo();
        $arDado['comboFiltro'] = $this->pegarCaminhoFiltro();
//         $arDado['comboPerfil'] = array(); //$perfil->listarDadosComboAgrupada();
        
        $this->visao->carregarDados($arDado);
        
        $this->visao->setarTituloCabecalho('Cadastro - Acesso Rápido');
        $this->visao->incluirVisao('formCadastro');
    }
    
    private function buscarDadoEdicao()
    {
        if ($_POST['acrid']) {
            require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido.php';
            require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido_Sistema.php';
            require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido_Perfil.php';
            
            $acessoRapido = new Seguranca_Model_AcessoRapido($_POST['acrid']);
            $d = $acessoRapido->getDados();

            $acessoRapido_Sistema = new Seguranca_Model_AcessoRapido_Sistema();
            $d['sisid'] = $acessoRapido_Sistema->pegarSisidPorAcrid($d['acrid']);
            
            $acessoRapido_Perfil = new Seguranca_Model_AcessoRapido_Perfil();
            $d['pflcod'] = $acessoRapido_Perfil->pegarPflcodPorAcrid($d['acrid']);
            
            echo simec_json_encode($d);
        }
    }
    
    private function inativar()
    {
        if ($_POST['acrid']) {
            require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido.php';
            
            $acessoRapido = new Seguranca_Model_AcessoRapido();
            $acessoRapido->apagarPorAcrid($_POST['acrid']);
            
            $acessoRapido->commit();
        }
    }
    
    private function salvar()
    {
        require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido.php';
        require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido_Sistema.php';
        require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido_Perfil.php';
        
        $acessoRapido        = new Seguranca_Model_AcessoRapido();
        $acrid = $acessoRapido->popularDadosObjeto($_POST)->salvar();
        
        $acessoRapido_Sistema = new Seguranca_Model_AcessoRapido_Sistema();
        $acessoRapido_Sistema->apagarPorAcrid($acrid);
        $_POST['sisid'] = ($_POST['sisid'] ? $_POST['sisid'] : array());
        foreach ($_POST['sisid'] as $sisid) {
            $dados = array(
                "acrid"  => $acrid,
                "sisid" => $sisid
            );
            $acessoRapido_Sistema->popularDadosObjeto($dados)->salvar();
            $acessoRapido_Sistema->clearDados();
        }
        
        $acessoRapido_Perfil = new Seguranca_Model_AcessoRapido_Perfil();
        $acessoRapido_Perfil->apagarPorAcrid($acrid);
        $_POST['pflcod'] = ($_POST['pflcod'] ? $_POST['pflcod'] : array());
        foreach ($_POST['pflcod'] as $pflcod) {
            $dados = array(
                "acrid"  => $acrid,
                "pflcod" => $pflcod
            );
            $acessoRapido_Perfil->popularDadosObjeto($dados)->salvar();
            $acessoRapido_Perfil->clearDados();
        }
        
        $acessoRapido_Perfil->commit();
    }
    
    private function buscarPerfil()
    {
        require_once APPRAIZ. 'seguranca/classes/model/Perfil.class.inc';
        
        $perfil  = new Seguranca_Model_Perfil();
        $dados = $perfil->acessoRapidoListarDadosComboAgrupada($_POST['sisid'], $_POST['sisdiretorio']);
        echo simec_json_encode(( $dados ));
        //                 echo simec_json_encode(simec_utf8_encode_recursive( $dados ));
    }
    
    private function buscarListaAcessoRapido()
    {
        require_once APPRAIZ. 'seguranca/classes/model/AcessoRapido.php';
        $dados = array();
        $acessoRapido  = new Seguranca_Model_AcessoRapido();
        $filtro = ($_POST['sisid'] ? ['s.sisid' => $_POST['sisid']] : []);
        $dados['listaAcessoRapido'] = $acessoRapido->listar($filtro);
        $dados['cabecalho'] = array('Ação', 'Nome do filtro', 'Módulo', 'Arquivo de filtro', 'Perfis que terão acesso', 'Ordem');
        
        $this->visao->carregarDados($dados)
                    ->incluirVisao('listaAcessoRapido', false);
    }
        
    private function pegarCaminhoFiltro()
    {
        $arResultado = array();
        
        $caminhoArquivoFiltro = APPRAIZ . 'includes/library/simec/AcessoRapido/filtro';
        $arDiretorio = dir($caminhoArquivoFiltro);
        while($diretorio = $arDiretorio->read()) {
            if ($diretorio == '.' || $diretorio == '..' || $diretorio == '.svn') {
                continue;
            }
            
            $caminhoArquivoFiltro = APPRAIZ . 'includes/library/simec/AcessoRapido/filtro/' . $diretorio;
            if (is_dir($caminhoArquivoFiltro)) {
                $arDiretorio_1 = dir($caminhoArquivoFiltro);
                while($diretorio_1 = $arDiretorio_1->read()) {
                    if ($diretorio_1 == '.' || $diretorio_1 == '..' || $diretorio_1 == '.svn') {
                        continue;
                    }
                    
                    $caminhoArquivoFiltro = APPRAIZ . 'includes/library/simec/AcessoRapido/filtro/' . $diretorio . '/' . $diretorio_1 . '/controle';
                    if (is_dir($caminhoArquivoFiltro)) {
                        $arDiretorio_2 = dir($caminhoArquivoFiltro);
                        while($diretorio_2 = $arDiretorio_2->read()) {
                            if ($diretorio_2 == '.' || $diretorio_2 == '..' || $diretorio_2 == '.svn') {
                                continue;
                            }
                            
                            $arResultado[] = array(
                                "codigo" => $diretorio . '/' . $diretorio_1 . '/controle/'. $diretorio_2,
                                "descricao" => $diretorio . '/' . $diretorio_1 . '/controle/'. $diretorio_2
                            );
                        }
                    }
                }
            }
        }
        
        return $arResultado;        
    }
}