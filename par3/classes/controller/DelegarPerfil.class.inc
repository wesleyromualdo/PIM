<?php
include_once APPRAIZ . 'par3/classes/model/seguranca/Perfil.class.inc';
include_once APPRAIZ . 'par3/classes/model/seguranca/PerfilVinculado.class.inc';
include_once APPRAIZ . 'includes/funcoesspo_componentes.php';

class Par3_Controller_DelegarPerfil extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Função formEquipeLocal
     * - monta o formulario de equipe local
     *
     */
    public function comboDelegarPerfis($codPerfil)
    {
        $listaPerfis = new Seguranca_Model_Seguranca_Perfil();
        $opcoes = array('multiple'=> 'multiple', 'titulo'=>'Selecione o(s) Perfil(s)', 'return' => 'return');

        return inputCombo('pflvinculados[]', $listaPerfis->listaPerfis(), $this->perfisVinculados($codPerfil), 'pflvinculados[]', $opcoes);
    }//end formDelegarPerfis()


    public function perfisVinculados($codPerfil)
    {
        $perfisVinculados = new Seguranca_Model_Seguranca_PerfilVinculado();

        $sql = "SELECT array_to_json(pflvinculado) AS vinculados FROM seguranca.perfilvinculado WHERE pflcod = {$codPerfil}";
        $listaPerfis = $perfisVinculados->recuperar($sql);

        return json_decode($listaPerfis['vinculados']);
    }

    public function listaPerfisVinculados()
    {
        $perfis      = $this->podemDelegar();
        $listaPerfis = array();

        foreach ($perfis as $perfil) {
            $listaPerfis[] = $this->perfisVinculados($perfil);
        }

        return $listaPerfis[0];
    }

    public function perfisUsuario()
    {
        global $db;
        $sql = "SELECT * FROM seguranca.perfilusuario where usucpf = '{$_SESSION['usucpf']}';";

        $usuarioPerfis = array();
        $perfisUsuario = $db->carregar($sql);
        foreach ( $perfisUsuario as $usuarioPerfil) {
            $usuarioPerfis[] = $usuarioPerfil['pflcod'];
        }

        return $usuarioPerfis;
    }

    public function podemDelegar()
    {
        global $db;
        $pflcod = array();

        foreach ($this->perfisUsuario() as $perfil) {
            $sql_delegar = "SELECT pflcod, pflinddelegar FROM seguranca.perfil where sisid = {$_SESSION['sisid']} AND pflcod = {$perfil}";
            $podeDelegar  = $db->carregar($sql_delegar);

            if ('t' == $podeDelegar[0]['pflinddelegar']) {
                $pflcod[] = $podeDelegar[0]['pflcod'];
            }
        }

        return $pflcod;
    }

        /**
     * Função salvar
     * - salvar relacionamento de perfis.
     */
    public function salvar($arrPost)
    {
        $url = "par3.php?modulo=sistema/perfil/cadperfil&acao=A";
        $modelPerfilVinculado = new Seguranca_Model_Seguranca_PerfilVinculado();

        try {
            $modelPerfilVinculado->salvarPerfilVinculado($arrPost['pflcod'], $arrPost['pflvinculados']);
            $modelPerfilVinculado->commit();

            simec_redirecionar($url, 'success');
        } catch(Exception $e) {
            $modelPerfilVinculado->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }

    }//end salvar()

    public function ativaMenus($pflcod, $insert = true)
    {
        global $db;
        $sql = "SELECT mnuid FROM seguranca.menu WHERE sisid = {$_SESSION['sisid']} 
                              AND mnudsc ILIKE ANY(ARRAY['sistema', 'usuários', 'gerenciar', 'alterar dados de usuário']);";
        $menus = $db->carregar($sql);

        foreach ($menus as $menu) {
            $query = "DELETE FROM seguranca.perfilmenu WHERE mnuid = '{$menu['mnuid']}' AND pflcod = {$pflcod};";
            $db->executar($query, false);

            if ($insert) {
                $query = "INSERT INTO seguranca.perfilmenu (pflcod, pmnstatus, mnuid) VALUES ( '{$pflcod}', 'A', '{$menu['mnuid']}' );";
                $db->executar($query, false);
            }
        }
    }

}//end class

