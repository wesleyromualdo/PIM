<?php
namespace AcessoRapido\filtro\par\entidade\controle;


class Entidade extends \AcessoRapido\core\Filtro
{
    public function getMunicipioFiltrado()
    { 
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . '/par/classes/modelo/Municipio.class.inc';
        
        $municipio = new \Municipio();
        $arDado['arrMunicipio'] = $municipio->acessoRapidoCarregarCombo($_POST, false);
        
        echo simec_json_encode( $arDado );
        die;
    }
    
    public function montarTela()
    {
        global $db;
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
            $tela               = "telaBootstrap.php";
            $dados['arrEsfera'] = [1 => 'Estadual',2 => 'Municipal'];
        } else {
            $tela               = "tela.php";
            $dados['arrEsfera'] = ['Estadual'  => ['id'=>'esfera_acessorapido_estadual', 'valor'=>1, 'default'=>true],
                                   'Municipal' => ['id'=>'esfera_acessorapido_municipal', 'valor'=>2]];
        }
        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . '/par/classes/modelo/Municipio.class.inc';
        include_once APPRAIZ . '/par/classes/modelo/Estado.class.inc';
        include_once APPRAIZ . 'www/par3/_funcoes.php';
        
        $estado = new \Estado();
        $municipio = new \Municipio();
        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'seguranca/classes/model/Menu.php';
        $menu = new \Seguranca_Model_Menu();
        $dados['menuAcessoDireto'] = $menu->listarMenuAcessoDireto(['m.sisid'=> 23, 'pu.usucpf'=>$_SESSION['usucpf']]);
        
        $dados['acrid']              = $_POST['acrid'];
        $dados['arrEstado']          = $estado->carregarUf(true);
        $dados['arrMunicipio']       = $municipio->acessoRapidoCarregarCombo([], true);
//         $dados['arrEstado']          = \Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
//         $dados['arrMunicipio']       = \Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa(true);
        
        $this->incluirVisao($tela, $dados);
    }
    
    public function aplicarFiltro()
    {        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'par/classes/modelo/UsuarioResponsabilidade.class.inc';
        
        $usuarioResponsabilidade = new \Par_Model_Usuarioresponsabilidade();
        $arPerfil = $usuarioResponsabilidade->acessoRapidoPegarPerfil($_SESSION['usucpf'], 23);
        $arPerfilComResponsabilidade = $_SESSION['PAR']['acessorapido'][23][$_SESSION['usucpf']]['pflcod_responsabilidade'];
        
        if (array_diff($arPerfil, $arPerfilComResponsabilidade) && count($arPerfilComResponsabilidade) > 0) {
            $arResponsabilidade = $_SESSION['PAR']['acessorapido'][23][$_SESSION['usucpf']]['responsabilidade'];
        } else {
            $arResponsabilidade = ["muncod"=>[], "estuf"=>[]];
        }
        
        if ($_POST['itrid'] == 1) {
            if (!in_array($_POST['estuf'], $arResponsabilidade['estuf']) && count($arResponsabilidade['estuf']) > 0) {
                die('Sem permissão para acessar esta entidade: ' . $_POST['estuf']);
            }
            include_once APPRAIZ . 'par/classes/modelo/Estado.class.inc';
            $estado = new \Estado();
            $dados = $estado->acessoRapidoPegarInstrumento($_POST);
        } else {
            include_once APPRAIZ . 'par/classes/modelo/Municipio.class.inc';
            $municipio = new \Municipio();
            
            if (!in_array($_POST['muncod'], $arResponsabilidade['muncod']) && count($arResponsabilidade['muncod']) > 0) {
                $descricaoMunicipio = $municipio->descricaoMunicipio($_POST['muncod'], true);
                die('Sem permissão para acessar esta entidade: ' . $descricaoMunicipio);
            }
            
            $dados = $municipio->acessoRapidoPegarInstrumento($_POST);
        }
        
        if (!$dados) {
            die('Entidade não encontrada.');
        }
        
        $_SESSION['par']['itrid']  = $dados['itrid'];
        $_SESSION['par']['inuid']  = $dados['inuid'];
        $_SESSION['par']['estuf']  = $dados['estuf'];
        $_SESSION['par']['muncod'] = $dados['muncod'];
        
        
        die("{\"link\":\"/par/par.php?modulo=principal/administracaoDocumentos&acao=A\"}");        
    }
}