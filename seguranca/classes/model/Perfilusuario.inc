<?php
/**
 * Classe de mapeamento da entidade seguranca.perfilusuario
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-12-2015
 * @link     no link
 */



/**
 * Perfilusuario
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-12-2015
 * @link     no link
 */
class Seguranca_Model_Perfilusuario extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'seguranca.perfilusuario';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array('usucpf');

    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'pflcod' => array('tabela' => 'perfil', 'pk' => 'pflcod'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'pflcod' => null,
    );

    public function salvarDados($dados = array())
    {
        $cpf = $dados['usucpf'];
        $this->executar("INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dados['usucpf']}', '{$dados['pflcod']}')");
        $this->commit();
    }

    /**
     * Desassocia um usuário de um conjunto de perfis.
     * @param string $usucpf Cpf do usuário que terá os perfis removidos.
     * @param array $perfis Conjunto de perfis para desassociação.
     */
    public function removerPerfis($usucpf, array $perfis = [])
    {
        if (!empty($perfis)) {
            $perfis = "'" . implode("', '", $perfis) . "'";
            $sql = <<<DML
DELETE
  FROM seguranca.perfilusuario
  WHERE usucpf = '{$usucpf}'
    AND pflcod IN({$perfis})
DML;
            $this->executar($sql);
            $this->commit();
        }

        return $this;
    }

    /**
     * Desassocia um usuário de um conjunto de perfis.
     * @param string $usucpf Cpf do usuário que terá os perfis removidos.
     * @param array $perfis Conjunto de perfis para desassociação.
     */
    public function atualizaPerfis($usucpf, array $arrPerfis = [])
    {
        if (!empty($arrPerfis)) {
            $perfis = "'" . implode("', '", $arrPerfis) . "'";
            $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '$usucpf' AND pflcod IN ($perfis);";
            foreach($arrPerfis as $pflcod){
	            $sql .= "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('$usucpf', '$pflcod');";
            }
            $this->executar($sql);
            $this->commit();
        }

        return $this;
    }

    public function pegaPerfis($usucpf, $sisid = null) {
        $sisid = empty($sisid) ? $_SESSION['sisid'] : $sisid;

        $sql = <<<SQL
            SELECT
              pfl.pflcod
            FROM seguranca.perfil pfl
              INNER JOIN seguranca.perfilusuario pfu ON pfu.pflcod = pfl.pflcod
              INNER JOIN seguranca.usuario usu ON usu.usucpf = pfu.usucpf
            WHERE pfl.sisid = {$sisid}
            AND usu.usucpf = '{$usucpf}';
SQL;

        return $this->carregar($sql);
    }

    /**
     * Desassocia um usuário de um conjunto de perfis.
     * @param string $usucpf Cpf do usuário que terá os perfis removidos.
     * @param array $perfis Conjunto de perfis para desassociação.
     */
    public function atualizaPerfisCacs($usucpf, array $arrPerfis = [], $sisid)
    {
        if($sisid == PAR3_SIS_ID){
            $arrPerfilCacs = array(
                Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS,
                Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS
            );
        } else {
            $arrPerfilCacs = array(
                PAR_PERFIL_CONSELHEIRO,
                PAR_PERFIL_PRESIDENTE
            );
        }

        if (!empty($arrPerfis)) {
            $perfis = "'" . implode("', '", $arrPerfilCacs) . "'";
            $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '$usucpf' AND pflcod IN ($perfis);";
            foreach($arrPerfis as $pflcod){
                $sql .= "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('$usucpf', '$pflcod');";
            }
            $this->executar($sql);
            $this->commit();
        }

        return $this;
    }
}
