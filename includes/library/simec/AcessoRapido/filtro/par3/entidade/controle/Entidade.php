<?php
namespace AcessoRapido\filtro\par3\entidade\controle;


class Entidade extends \AcessoRapido\core\Filtro
{
    public function getMunicipioFiltrado()
    {
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'par3/classes/model/IniciativaPlanejamento.class.inc';
        
        $municipio = new \Par3_Model_IniciativaPlanejamento();
        $arDado['arrMunicipio'] = $municipio->acessoRapidoCarregarComboMunicipio($_POST, false);
        
        echo simec_json_encode( $arDado );
        die;
    }
    
    public function montarTela()
    {
        global $db;
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
            $tela = "telaBootstrap.php";
            $dados['arrEsfera']          = [1 => 'Estadual',2 => 'Municipal'];
        } else {
            $tela = "tela.php";
            $dados['arrEsfera']          = ['Estadual' => ['id'=>'esfera_acessorapido_estadual', 'valor'=>1, 'default' => true],
                                            'Municipal' => ['id'=>'esfera_acessorapido_municipal', 'valor'=>2]];
        }
        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'par3/classes/model/IniciativaPlanejamento.class.inc';
        
        $dados['arrEstado']          = \Par3_Model_IniciativaPlanejamento::getEstadoMunicipioPorIniciativa();
        $dados['arrMunicipio']       = \Par3_Model_IniciativaPlanejamento::acessoRapidoCarregarComboMunicipio([], true);
        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'seguranca/classes/model/Menu.php';
        $menu = new \Seguranca_Model_Menu();
        $dados['menuAcessoDireto'] = $menu->listarMenuAcessoDireto(['m.sisid'=> 231, 'pu.usucpf'=>$_SESSION['usucpf']]);
        
        $this->incluirVisao($tela, $dados);
    }
    
    public function aplicarFiltro()
    {        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'par3/classes/model/InstrumentoUnidade.class.inc';
        include_once APPRAIZ . 'www/par3/_constantes.php';
//         initAutoload();
        
        $modelInstrumentoUnidade = new \Par3_Model_InstrumentoUnidade();
        $inuid = $modelInstrumentoUnidade->pegarInuidAcessivel($_POST);
        
        if ($inuid) {
            die("{\"link\":\"/par3/par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$inuid}\"}");
        } else {
            die("Entidade não encontrada.");
        }
    }
}