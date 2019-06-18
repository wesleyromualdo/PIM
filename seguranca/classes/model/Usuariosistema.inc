<?php
/**
 * Classe de mapeamento da entidade seguranca.usuario_sistema
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-12-2015
 * @link     no link
 */


/**
 * Usuario_sistema
 *
 * @category Class
 * @package  A1
 * @author   MARCUS VINÍCIUS LOUVEM ROCHA <marcus.rocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 04-12-2015
 * @link     no link
 */
class Seguranca_Model_Usuariosistema extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'seguranca.usuario_sistema';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'usucpf',
        'sisid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'sisid' => array('tabela' => 'sistema', 'pk' => 'sisid'),
        'usucpf' => array('tabela' => 'usuario', 'pk' => 'usucpf'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'usucpf' => null,
        'usucpf' => null,
        'sisid' => null,
        'sisid' => null,
        'susstatus' => null,
        'pflcod' => null,
        'susdataultacesso' => null,
        'suscod' => null,
    );

    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'usucpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'sisid' => array('Digits'),
            'sisid' => array('Digits'),
            'susstatus' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
            'pflcod' => array('allowEmpty' => true, 'Digits'),
            'susdataultacesso' => array('allowEmpty' => true),
            'suscod' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 1))),
        );
    }//end getCamposValidacao($dados)

    public function salvar($dados = array(), $autoCommit = true)
    {
        $cpf = $dados['usucpf'];
        $pflcod = ($dados['pflcod'] ? $dados['pflcod'] : 'null');
        $sisid = $dados['sisid'];

        $verificaUserSistema = $this->pegaUm("SELECT usucpf FROM seguranca.usuario_sistema WHERE usucpf = '$cpf' AND sisid = '$sisid'");

        if (empty($verificaUserSistema)) {
            $this->executar("INSERT INTO seguranca.usuario_sistema (usucpf, sisid, pflcod) VALUES ('$cpf', '{$dados['sisid']}', {$pflcod})");
            $this->commit();
        } else {
            return true;
        }
    }

    public function salvarUsuarioSistema($cpf, $sisid)
    {
        $this->usucpf = $cpf;
        $this->sisid = $sisid;
        $this->suscod = 'A';

        $sql = " INSERT INTO seguranca.usuario_sistema( usucpf, sisid, suscod) VALUES ('{$this->usucpf}', '{$this->sisid}', '{$this->suscod}') returning usucpf; ";
        return $this->executar($sql, false);
    }

    /**
     * Tenta atualizar o estado de um usuário em um sistema para ativo, se não conseguir, insere uma nova associação.
     *
     * @param type $usucpf
     * @param type $sisid
     */
    public function ativaUsuario($usucpf, $sisid)
    {
        $sql = <<<DML
UPDATE seguranca.usuario_sistema
  SET suscod = 'A'
  WHERE usucpf = '{$usucpf}'
    AND sisid = '{$sisid}'
  RETURNING usucpf
DML;
        if (!$this->pegaUm($sql)) {
            $this->salvarUsuarioSistema($usucpf, $sisid);
        }
    }

    public function bloqueiaUsuario($usucpf, $sisid)
    {
        $sql = "UPDATE seguranca.usuario_sistema
                  SET suscod = 'B'
                  WHERE usucpf = '{$usucpf}'
                    AND sisid = '{$sisid}'
                ";
        return $this->executar($sql);
    }

}
