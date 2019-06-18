<?php
namespace AcessoRapido;

final class Controlador
{
    public static $instancia;
    private $db;
    private $pathVisao;
    private $pathFiltro;
        
    private function __construct()
    {
        $this->db        = new \cls_banco();
        $this->pathCore  = dirname(__FILE__);
        $this->pathVisao = dirname(__FILE__) . '/visao/';
        $this->pathFiltro= dirname(dirname(__FILE__)). '/filtro/';
        
        $this->montarTelaFiltro();
        $this->aplicarFiltro();
        $this->aplicarAcaoEspecifica();
    }
    
    public static function pegarInstancia() {
        if (!isset(self::$instancia)) {
            self::$instancia = new Controlador();
        }
        
        return self::$instancia;
    }
    
    public function montarCaixa()
    {
        $dados = array();
        $dados['menus'] = $this->listarAcessoRapido();
        
        if (!$dados['menus']) {
            return false;
        }
        
        $dados['arClassItem'] = array(
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin',
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin',
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin'
        );
        
        $this->incluirVisao('menu.php', $dados);
        $this->incluirJsCore();
    }
    
    public function montarCaixaBootstrap()
    {
        $dados = array();
        $dados['menus'] = $this->listarAcessoRapido();
        
        if (!$dados['menus']) {
            return false;
        }
        
        $dados['arClassItem'] = array(
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin',
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin',
            'default-skin',
            'blue-skin',
            'yellow-skin',
            'ultra-skin'
        );

        $this->incluirVisao('menuBootstrap.php', $dados);
        $this->incluirJsCore();
    }
    
    private function aplicarAcaoEspecifica()
    {
        if ($_POST['requisicao'] == 'acessoRapidoAplicarAcaoEspecifica' && $_POST['acrid']) {
            $dados = $this->pegarAcessoRapidoPorId($_POST['acrid']);
            
            if (!$dados['acrid']) {
                die('Filtro não encontrado: Controlador::aplicarAcaoEspecifica(...)');
            }
            
            require_once $this->pathCore . '/Filtro.php';
            require_once $this->pathFiltro . $dados['acrfiltro'];
            
            $nomeObj = $this->prepararNomeObjetoControleFiltro($dados['acrfiltro']);
            
            $filtro = new $nomeObj();
            $metodo = $_REQUEST['metodo'];
            if (method_exists($filtro, $metodo)) {
                $filtro->$metodo();
            } else {
                die("Método inexistente: {$nomeObj}::{$metodo}(...)");
            }
            die;
        }
    }
    
    private function aplicarFiltro()
    {
        if ($_POST['requisicao'] == 'acessoRapidoAplicarFiltro' && $_POST['acrid']) {
            $dados = $this->pegarAcessoRapidoPorId($_POST['acrid']);
            
            if (!$dados['acrid']) {
                die('Filtro não encontrado');
            }
            
            require_once $this->pathCore . '/Filtro.php';
            require_once $this->pathFiltro . $dados['acrfiltro'];
            
            $nomeObj = $this->prepararNomeObjetoControleFiltro($dados['acrfiltro']);
            
            $filtro = new $nomeObj();
            $filtro->aplicarFiltro();
            die;
        }
    }
    
    private function montarTelaFiltro()
    {
        if ($_POST['requisicao'] == 'acessoRapidoTelaFiltro' && $_POST['acrid']) {
            $dados = $this->pegarAcessoRapidoPorId($_POST['acrid']);
            
            if (!$dados['acrid']) {
                die('Filtro não encontrado (montarTelaFiltro()). ');
            }
            
            require_once $this->pathCore . '/Filtro.php';
            require_once $this->pathFiltro . $dados['acrfiltro'];
            
            $nomeObj = $this->prepararNomeObjetoControleFiltro($dados['acrfiltro']);
            
            $filtro = new $nomeObj();
            $filtro->montarTela();
            die;
        }
    }
    
    private function prepararNomeObjetoControleFiltro( $path )
    {
        $path = str_replace("/", "\\", $path);
        $path = str_replace(".php", "", $path);
        $path = '\AcessoRapido\filtro\\' . $path;
        
        return $path;
    }
    
    private function pegarAcessoRapidoPorId($acrid)
    {
        $sql    = "SELECT
                        ar.acrid, ar.acrdsc, acrfiltro
                   FROM
                        seguranca.acessorapido ar
                        join seguranca.acessorapido_sistema ars on ars.acrid = ar.acrid and 
                                                                   ars.sisid={$_SESSION['sisid']}
                        join seguranca.acessorapido_perfil ap on ap.acrid = ar.acrid
                        join seguranca.perfilusuario pu on pu.pflcod = ap.pflcod and 
                                                           pu.usucpf = '{$_SESSION['usucpf']}'
                   WHERE
                        ar.acrstatus='A' AND
                        ar.acrid={$acrid}";
        $dados  = $this->db->pegaLinha($sql);
        $dados  = ($dados ? $dados : array());
        
        return $dados;
    }
    
    private function listarAcessoRapido()
    {
        $sql    = "SELECT 
                        distinct
                        ar.acrid, ar.acrdsc, ar.acrordem 
                   FROM 
                        seguranca.acessorapido ar
                        join seguranca.acessorapido_sistema ars on ars.acrid = ar.acrid and 
                                                                   ars.sisid={$_SESSION['sisid']}
                        join seguranca.acessorapido_perfil ap on ap.acrid = ar.acrid
                        join seguranca.perfilusuario pu on pu.pflcod = ap.pflcod and pu.usucpf = '{$_SESSION['usucpf']}'
                   WHERE 
                        ar.acrstatus='A' 
                   ORDER BY
                        ar.acrordem asc";
        $dados  = $this->db->carregar($sql);
        $dados  = ($dados ? $dados : array());
        
        return $dados;
    }
        
    private function incluirJsCore()
    {
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            echo '<script language="JavaScript" src="../includes/AcessoRapido/coreBootstrap.js?'. rand(1, 100) .'"></script>';
        } else {
            echo '<script language="JavaScript" src="../includes/AcessoRapido/core.js?'. rand(1, 100) .'"></script>';            
        }
    }
    
    private function incluirVisao($arquivo, $dados)
    {
        require_once $this->pathVisao . "{$arquivo}";
    }
    
}