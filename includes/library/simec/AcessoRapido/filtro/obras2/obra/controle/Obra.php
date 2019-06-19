<?php
namespace AcessoRapido\filtro\obras2\obra\controle;

class Obra extends \AcessoRapido\core\Filtro
{    
    public function montarTela()
    {   
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'seguranca/classes/model/Menu.php';
        $menu = new \Seguranca_Model_Menu();
        $dados['menuAcessoDireto'] = $menu->listarMenuAcessoDireto(['m.sisid'=> 147, 'pu.usucpf'=>$_SESSION['usucpf']]);
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
            $tela = "telaBootstrap.php";
        } else {
            $tela = "tela.php";            
        }
        
        $this->incluirVisao($tela, $dados);
    }
    
    public function aplicarFiltro()
    {        
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'obras2/classes/modelo/UsuarioResponsabilidade.class.inc';
        include_once APPRAIZ . 'obras2/classes/modelo/Obras.class.inc';
        
        $_SESSION['obras2']['orgid']    = 3;
        $usucpf                         = $_SESSION['usucpf'];
        $sisid                          = 147;
        
        $usuarioResponsabilidade = new \UsuarioResponsabilidade();
        
        $arPerfil = $usuarioResponsabilidade->acessoRapidoPegarPerfil($usucpf, $sisid);
        $arPerfilComResponsabilidade = $_SESSION['OBRAS2']['acessorapido'][$sisid][$usucpf]['pflcod_responsabilidade'];
        $arResponsabilidade = $_SESSION['OBRAS2']['acessorapido'][$sisid][$usucpf]['responsabilidade'];
        
        $obras = new \Obras();
        if (array_diff($arPerfil, $arPerfilComResponsabilidade) && count($arPerfilComResponsabilidade) > 0) {
            $obrid = $obras->acessoRapidoPegarObra(["obrid"=>$_POST['obrid']], $arResponsabilidade);
        } else {
            $obrid = $obras->acessoRapidoPegarObra(["obrid"=>$_POST['obrid']], []);
        }
        
        if ($obrid) {
            die("{\"link\":\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid={$obrid}\"}");
        } else {
            die("Obra não encontrada.");
        }
    }
    
    public function montarTelaListaObraDetalhada()
    {
        include_once APPRAIZ . "includes/library/simec/Listagem.php";
//         include_once APPRAIZ . "includes/library/simec/Listagem/Datasource/Query.php";
        include_once APPRAIZ . "includes/classes/Modelo.class.inc";
        include_once APPRAIZ . 'obras2/classes/modelo/UsuarioResponsabilidade.class.inc';
        include_once APPRAIZ . 'obras2/classes/modelo/Obras.class.inc';
        
        if ($_SESSION['sislayoutbootstrap'] == 'zimec') {
            include_once APPRAIZ . 'includes/library/simec/view/Helper.php';
            $tela = "listaDetalheObraBootstrap.php";
            $executarSql = false;
        } else {
            $tela = "listaDetalheObra.php";
            $executarSql = true;
        }
        
        $_SESSION['obras2']['orgid']    = 3;
        $usucpf                         = $_SESSION['usucpf'];
        $sisid                          = 147;
        
        $usuarioResponsabilidade = new \UsuarioResponsabilidade();
        
        $arPerfil = $usuarioResponsabilidade->acessoRapidoPegarPerfil($usucpf, $sisid);
        $arPerfilComResponsabilidade = $_SESSION['OBRAS2']['acessorapido'][$sisid][$usucpf]['pflcod_responsabilidade'];
        $arResponsabilidade = $_SESSION['OBRAS2']['acessorapido'][$sisid][$usucpf]['responsabilidade'];
        
        $obras = new \Obras();
        if (array_diff($arPerfil, $arPerfilComResponsabilidade) && count($arPerfilComResponsabilidade) > 0) {
            $dados = $obras->acessoRapidoPegarListaDetalheObra(["obrid"=>$_POST['obrid']], $arResponsabilidade, $executarSql);
        } else {
            $dados = $obras->acessoRapidoPegarListaDetalheObra(["obrid"=>$_POST['obrid']], [], $executarSql);
        }
        
        $cabecalho = array(
            "ID", 
            "ID Pré-obra", 
            "Nº Processo",
            "Nº / Ano do termo / convênio",
            "Obra", 
            "Unidade implantadora", 
            "Município / UF", 
            "Data de início da execução", 
            "Situação da Obra",
            "Última vistoria instituição",
            "% executado empresa",
            "Tipologia"
        );
        
        $this->incluirVisao($tela, ['sqlListaDetalheObra'=>$dados, 'cabecalho'=>$cabecalho, 'campoOculto'=>['ultima_atualizacao', 'strid']]);
    }
    
    public static function tratarDataUltimaVistoria($dtvistoria, $args)
    {
        $estiloSituacaoObra = "";
        $corDataVistoria = "";
        
        // Definindo a cor da data da vistoria e identificando se a obra está desatualizada.
        if ($args["strid"] == 4 || $args["strid"] == 5) {
            if ($args["ultima_atualizacao"] <= 25) {
                $corDataVistoria = "#3CC03C";
            } elseif ($args["ultima_atualizacao"] > 25 && $args["ultima_atualizacao"] <= 30) {
                $corDataVistoria = "#FAD200";
            } else {
                $corDataVistoria = "#FF0000";
            }
        } elseif ($args["strid"] == 6) {
            $corDataVistoria = "#0066CC";
//             $estiloSituacaoObra = "style='color:#0066CC;font-weight:bold'";
        }
        
        // Formatando a data de vistoria.
        $dataUltimaVistoria = "";
        if ($args["dtvistoria"]) {
            /*
             * O tratamento a seguir é feito porque algumas datas vem da base de dados
             * com os microsegundos. Isso causava erro.
             */
            if (strpos($args["dtvistoria"], ".")) {
                $tmp = explode(".", $args["dtvistoria"]);
                $dtUltVistoria = $tmp[0];
            } else {
                $dtUltVistoria = $args["dtvistoria"];
            }
            /* Fim do tratamento da data. */
            
            $objDateTime = \DateTime::createFromFormat("Y-m-d H:i:s", $dtUltVistoria);
            $dataUltimaVistoria = $objDateTime->format("d/m/Y");
            $dataUltimaVistoria = $dataUltimaVistoria . "<br/>(" . $args["ultima_atualizacao"] . " dia(s))";
        }
        
        return "<div style=\"color:{$corDataVistoria}\">{$dataUltimaVistoria}</div>";   
    }
    
    public static function tratarSituacaoObra($dtvistoria, $args)
    {
        $estiloSituacaoObra = "";
        
        // Definindo a cor da data da vistoria e identificando se a obra está desatualizada.
        if ($args["strid"] == 6) {
            $estiloSituacaoObra = "style='color:#0066CC;font-weight:bold'";
        }
     
        return "<div {$estiloSituacaoObra}>{$args["situacao"]}</div>";
    }
}